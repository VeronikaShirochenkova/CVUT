package cz.cvut.fel.pjv;

public class NodeImpl implements Node {

    public Node left_node;
    public Node right_node;
    public int value;

    public NodeImpl(int value) {
        this.value = value;
        this.right_node = null;
        this.left_node = null;
    }

    @Override
    public Node getLeft() {
        return this.left_node;
    }

    @Override
    public Node getRight() {
        return this.right_node;
    }

    @Override
    public int getValue() {
        return this.value;
    }
}