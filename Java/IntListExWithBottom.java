/* 
 * J2100178 吉田安里
 * 第4回 課題2
 */

public class IntListExWithBottom extends IntListEx {
    IntNode top;
    IntNode bottom;

    IntListExWithBottom() {
        super();
    }

    @Override
    void addFirst(int val) {
        if (top == null) {
            top = new IntNode(val, null);
            bottom = top;
        } else {
            top = new IntNode(val, top);
        }
    }

    @Override
    void addLast(int val) {
        if (top == null) {
            top = new IntNode(val, null);
            bottom = top;
        } else {
            bottom.next = new IntNode(val, null);
            bottom = bottom.next;
        }
    }

    @Override
    int getLast() {
        if (top == null)
            return Integer.MIN_VALUE;
        else
            return bottom.val;
    }

    @Override
    void removeLast() {
        if (top == null) {
            return;
        }
        else if (top == bottom) {
            top = null;
            bottom = null;
            return;
        } else {
            IntNode node = top;
            while (node.next != bottom) {
                node = node.next;
            }
            bottom = node;
            bottom.next = null;
            return;
        }
    }

    boolean isEmpty() {
        return top == null;
    }

    public static void main(String[] args) {
        IntListExWithBottom list = new IntListExWithBottom();
        list.addFirst(5);
        System.out.println(list.getLast());
        list.addFirst(6);
        System.out.println(list.getLast());
        list.addLast(9);
        System.out.println(list.getLast());
        list.addLast(2);
        System.out.println(list.getLast());

        System.out.println();

        while (!list.isEmpty()) {
            System.out.println(list.getLast());
            list.removeLast();
        }
    }
}
