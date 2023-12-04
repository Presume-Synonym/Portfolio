/* プログラミング言語4 第04回 課題用配布ソースコード */
public class IntListEx {
    IntNode top;

    IntListEx() {
        top = null;
    }

    void addFirst(int val) {
        top = new IntNode(val, top);
    }

    void addLast(int val) {
        if (top == null) {
            top = new IntNode(val, null);
        } else {
            IntNode node = top;
            while (node.next != null) {
                node = node.next;
            }
            node.next = new IntNode(val, null);
        }
    }

    void print() {
        for (IntNode node = top; node != null; node = node.next) {
            System.out.print(node.val + " ");
        }
        System.out.println();
    }

    int getFirst() {
        if (top == null) 
            return Integer.MIN_VALUE;
        else
            return top.val;
    }

    int getLast() {
        if (top == null)
            return Integer.MIN_VALUE;
        
        IntNode node = top;
        while (node.next != null) {
            node = node.next;
        }
        return node.val;
    }

    int getAt(int index) {
        if (top == null) {
            return Integer.MIN_VALUE;
        }

        for (IntNode node = top; node != null; node = node.next) {
            if (index == 0) {
                return node.val;
            }
            
            index--;
        }
        return Integer.MIN_VALUE;
    }

    void removeFirst() {
        if (top == null) {
            return;
        }

        top = top.next;
    }

    void removeLast() {
        if (top != null) {
            if (top.next == null) {
                top = null;
            } else {
                IntNode prev = top;
                IntNode node = prev.next;
                while (node.next != null) {
                    prev = node;
                    node = node.next;
                }
                // この時点で prev.next は最後尾のオブジェクトを指している
                prev.next = null;    
            }
        }
    }

    int size() {
        int n = 0;
        for (IntNode node = top; node != null; node = node.next) {
            n++;
        }
        return n;
    }

    public static void main(String[] args) {
        IntListEx list = new IntListEx();
        list.addFirst(5);
        list.addFirst(6);
        list.addLast(9);
        list.addLast(2);
        list.addFirst(3);

        list.print();
    }
}

class IntNode {
    int val;
    IntNode next;

    IntNode(int val, IntNode node) {
        this.val = val;
        this.next = node;
    }
}
