package cz.cvut.fel.pjv;

/**
 * Implementation of the {@link Queue} backed by fixed size array.
 */
public class CircularArrayQueue implements Queue {

    /**
     * Creates the queue with capacity set to the value of 5.
     */
    String myQueue[];
    int head, tail, counter, maxsize;

    public CircularArrayQueue() {
        myQueue = new String[5];
        head = 0;
        tail = 0;
        counter = 0;
        maxsize = 5;
    }

    /**
     * Creates the queue with given {@code capacity}. The capacity represents
     * maximal number of elements that the queue is able to store.
     *
     * @param capacity of the queue
     */
    public CircularArrayQueue(int capacity) {
        myQueue = new String[capacity];
        head = 0;
        tail = 0;
        counter = 0;
        maxsize = capacity;
    }

    @Override
    public int size() {
        return counter;
    }

    @Override
    public boolean isEmpty() {
        if (counter == 0) {
            return true;
        } else {
            return false;
        }

    }

    @Override
    public boolean isFull() {
        if (counter == maxsize) {
            return true;
        } else {
            return false;
        }
    }

    @Override
    public boolean enqueue(String obj) {
        if (counter != maxsize & obj != null) {
            if (tail == maxsize) {
                tail = 0;
                myQueue[tail] = obj;
                tail++;
                counter++;
            } else {
                myQueue[tail] = obj;
                tail++;
                counter++;
            }
            return true;
        } else {
            return false;
        }
    }

    @Override
    public String dequeue() {
        if (!isEmpty()) {
            head++;
            counter--;
            if (head == maxsize) {
                head = 0;
                return myQueue[maxsize - 1];
            }
            return myQueue[head - 1];
        } else {
            return null;
        }
    }

    @Override
    public void printAllElements() {
        for (int i = head; i < tail; i++) {
            System.out.println(myQueue[i]);
        }
    }
}
