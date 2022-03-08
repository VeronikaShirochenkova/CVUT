package cz.cvut.fel.pjv;

public class TreeImpl implements Tree {

    Node root;
    int[] values;

    public Node search_node(int first_idx, int last_idx) {

        if (first_idx == last_idx) {
            return new NodeImpl(this.values[first_idx]);
        } else {
            int c = center(first_idx, last_idx);
            NodeImpl node = new NodeImpl(this.values[c]);

            if (first_idx < last_idx) {
                node.left_node = search_node(first_idx, c - 1);
            }

            if (c + 1 <= last_idx) {
                node.right_node = search_node(c + 1, last_idx);
            }
            return node;
        }
    }

    public int center(int first_idx, int last_idx) {
        int length = last_idx - first_idx + 1;
        return first_idx + (length / 2);
    }

    @Override
    public void setTree(int[] values) {
        this.values = values;

        if (values.length == 0) {
            this.root = null;
        } else {
            int first_idx = 0;
            int last_idx = values.length - 1;
            this.root = search_node(first_idx, last_idx);
        }
    }

    @Override
    public Node getRoot() {
        return this.root;
    }

    @Override
    public String toString() {

        if (this.root == null) {
            return "";
        } else {
            return print(this.root, 0);
        }
    }

    private String print(Node n, int depth) {
        String string = "";
        for (int i = 0; i < depth; i++) {
            string += " ";
        }
        string += "- " + (n.getValue()) + "\n";
        if (n.getLeft() != null) {
            string += print(n.getLeft(), depth + 1);
        }
        if (n.getRight() != null) {
            string += print(n.getRight(), depth + 1);
        }
        return string;
    }
}