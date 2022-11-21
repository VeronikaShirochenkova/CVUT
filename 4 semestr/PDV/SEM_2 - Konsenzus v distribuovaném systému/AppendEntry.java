package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;

class AppendEntry extends Message {
    int term;
    String leader_id;

    AppendEntry(int term, String id) {
        this.term = term;
        this.leader_id = id;
    }
}
