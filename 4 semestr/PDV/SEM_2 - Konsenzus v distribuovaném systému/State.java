package cz.cvut.fel.agents.pdv.student;
import cz.cvut.fel.agents.pdv.raft.messages.IOperation;

public enum State{
    FOLLOWER,
    CANDIDATE,
    LEADER;
}
