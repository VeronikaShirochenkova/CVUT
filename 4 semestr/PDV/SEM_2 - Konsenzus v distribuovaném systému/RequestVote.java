package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;

class RequestVote extends Message {
    int term;
    String candidate_id;

    RequestVote(int term, String id) {
        this.term = term;
        this.candidate_id = id;
    }
}
