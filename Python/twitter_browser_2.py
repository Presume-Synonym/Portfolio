import sys
import utils
import tweepy
import pytz
import time
from datetime import datetime

screen_name = ''
password = ''
api = utils.get_cookie_api(screen_name, password)

def get_tweet_origin_user(tweet_id):
    try:
        # ツイート情報を取得
        tweet = api.get_status(tweet_id, tweet_mode='extended')

        # ツイート元のアカウントのスクリーンネームを取得
        origin_user_screen_name = tweet.user.screen_name
        return origin_user_screen_name
    except tweepy.TweepyException as e:
        print(f"エラー：{e}")
        return None

def convert_to_jst(utc_time):
    utc_time = datetime.strptime(utc_time, "%Y-%m-%d %H:%M:%S%z")
    jst = pytz.timezone('Asia/Tokyo')
    jst_time = utc_time.astimezone(jst)
    return jst_time.strftime('%Y-%m-%d %H:%M:%S')

def display_tweet(tweet):
    tweet_time = str(tweet.created_at)
    tweet_time = convert_to_jst(tweet_time)
    # tweet_time = tweet.created_at.strftime("%Y-%m-%d %H:%M:%S")
    tweet_text = tweet.full_text
    retweets_count = tweet.retweet_count
    likes_count = tweet.favorite_count
    screenname = tweet.user.screen_name
    nickname = tweet.user.name
    is_protected = tweet.user.protected
    is_retweeted = hasattr(tweet, 'retweeted_status')

    print(f"--- {tweet_time} ---")
    if is_retweeted:
        retweet_user_name = tweet.retweeted_status.user.screen_name
        retweet_user_nickname = tweet.retweeted_status.user.name
        print(f"{nickname}{'🔒' if is_protected else ''} @{screenname} がリツイートしました")
        print(f"    ------")
        print(f"    {retweet_user_nickname} @{retweet_user_name}")
        print(f"{tweet_text}")
        print(f"    ------")
    else:
        if tweet.in_reply_to_status_id is not None:
            replyed_tweet = tweet.in_reply_to_status_id
            replyed_user = get_tweet_origin_user(replyed_tweet)
            print(f"--{replyed_user}へ返信しました--")
        print(f"{nickname}{'🔒' if is_protected else ''}  @{screenname}")
        print()
        print(f"{tweet_text}")

    print(f"リツイート数: {retweets_count}, いいね数: {likes_count}")
    print("------------------")
    print()


def display_home_timeline(count=10):
    try:
        # ユーザーのタイムラインを取得
        tweets = api.home_timeline(count=count, tweet_mode='extended')

        for tweet in tweets:
            display_tweet(tweet)

    except tweepy.TweepyException as e:
        print(f"エラー：{e}")

def display_user_timeline(username, count=10):
    try:
        # ユーザーのタイムラインを取得
        tweets = api.user_timeline(screen_name=username, count=count, tweet_mode='extended')

        for tweet in tweets:
            display_tweet(tweet)

    except tweepy.TweepyException as e:
        print(f"エラー：{e}")

def display_searched_timeline(username, search_string, count=1000):
    tweets = []
    max_tweets = 200
    last_tweet_id = None
    try:
        # ユーザーのタイムラインを取得
        while len(tweets) < count:
            new_tweets = api.user_timeline(screen_name=username, count=max_tweets, max_id=last_tweet_id, tweet_mode='extended')
            if new_tweets is None:
                break
            tweets.extend(new_tweets)
            last_tweet_id = tweets[-1].id - 1

        sys.stderr.write(f"{len(tweets)}件のツイートから検索します...\n\n")
        for tweet in tweets:
            if search_string in tweet.full_text:
                display_tweet(tweet)

    except tweepy.TweepyException as e:
        print(f"エラー：{e}")


def many_tweets(username, count, last):
    flag = 0
    last_len = 0
    tweets = []
    max_tweets = 200
    last_tweet_id = int(last)
    try:
        start = time.time_ns()
        # ユーザーのタイムラインを取得
        while len(tweets) < count:
            new_tweets = api.user_timeline(screen_name=username, count=max_tweets, max_id=last_tweet_id, tweet_mode='extended')
            sys.stderr.write(f"\r" + " " * 100)
            sys.stderr.write(f"\r{len(tweets)/count*100}% {(time.time_ns() - start)/1000000000}秒で{len(tweets)}件のツイートを取得しました...")
            tweets.extend(new_tweets)
            if last_len == len(tweets) and len(tweets) != 0:
                flag = 1
                sys.stderr.write(f"{tweets[-1].id - 1}")
                break
            # elif len(tweets) == 0:
            #     sys.stderr.write("ツイートの取得に失敗しました\n")
            #     exit(1)
            last_tweet_id = tweets[-1].id - 1
            last_len = len(tweets)
        end = time.time_ns()
        sys.stderr.write(f"\n{'指定された数のツイートの取得に失敗しました。 ' if flag else ''}{(end - start)/1000000000}秒で{len(tweets)}件のツイートを取得しました\n\n")
        sys.stderr.write(f"{'次回実行時に次のIDを入力してください: ' if flag else ''}{last_tweet_id if flag else ''}")
        for tweet in tweets:
            display_tweet(tweet)

    except tweepy.TweepyException as e:
        print(f"エラー：{e}")

def convert_time(data):
    if isinstance(data, dict):
        for key, value in data.items():
            if key == 'reset':
                data[key] = convert_to_jst(f"{datetime.fromtimestamp(value).strftime('%Y-%m-%d %H:%M:%S')}+0000")
            elif isinstance(value, (dict, list)):
                convert_time(value)
    elif isinstance(data, list):
        for item in data:
            convert_time(item)

def display_info(data):
    if isinstance(data, dict):
        data.items()


if __name__ == "__main__":
    limit_status = api.rate_limit_status()
    convert_time(limit_status)

    tweet_count = 15000
    last_id = 0
    sys.stderr.write("1: 自分のタイムラインを表示\n")
    sys.stderr.write("2: 特定ユーザーのタイムラインを表示\n")
    while True:
        sys.stderr.write("選択肢を入力してください（1または2）：")
        choice = input()
        if choice == '1':
            sys.stderr.write("自分のタイムラインを表示します：\n\n")
            display_home_timeline(count=tweet_count)
            break
        elif choice == '2':
            sys.stderr.write("表示したいユーザー名を入力してください：")
            username = input()
            sys.stderr.write("検索を行いますか？(y/n) ")
            is_search = input()
            if is_search == 'y':
                sys.stderr.write(f"{username} のツイートを検索します：\n\n")
                sys.stderr.write("検索する文字列を入力してください： ")
                search_string = input()
                sys.stderr.write("検索するツイート数を入力してください： ")
                tweet_count = int(input())
                display_searched_timeline(username, search_string, count=tweet_count)
            elif is_search == 'n':
                sys.stderr.write("前回実行時にIDが発行されましたか？(y/n) ")
                is_last = input()
                if is_last == 'y':
                    sys.stderr.write("発行されたIDを入力してください: ")
                    last_id = input()
                sys.stderr.write(f"{username} のタイムラインを表示します：\n\n")
                many_tweets(username, count=tweet_count, last=last_id)
            break
        else:
            print("無効な選択です。もう一度入力してください。")
    # display_timeline_tweets(tweet_count)