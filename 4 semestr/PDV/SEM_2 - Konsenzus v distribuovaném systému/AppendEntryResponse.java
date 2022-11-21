package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;

class AppendEntryResponse extends Message {
    int term;

    AppendEntryResponse(Integer term) {
        this.term = term;
    }
}
