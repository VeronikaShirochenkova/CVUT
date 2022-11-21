package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;

public class PingRequestMessage extends Message {

    private final String processID;

    public PingRequestMessage(String processID) {
        this.processID = processID;
    }

    public String getProcessID() {
        return processID;
    }

    @Override
    public String toString() {
        return "PINGREQ " + this.processID;
    }
}