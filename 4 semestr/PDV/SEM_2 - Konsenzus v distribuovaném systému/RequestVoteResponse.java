package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;

class RequestVoteResponse extends Message {
    int term;
    String candidate_id;

    RequestVoteResponse(Integer term, String id) {
        this.term = term;
        this.candidate_id = id;
    }
}
