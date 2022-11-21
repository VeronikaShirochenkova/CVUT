package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.raft.RaftProcess;

import java.util.*;
import java.util.concurrent.ThreadLocalRandom;
import java.util.function.BiConsumer;

import static java.lang.Math.max;

/**
 * Vasim ukolem bude naimplementovat (pravdepodobne nejenom) tuto tridu. Procesy v clusteru pracuji
 * s logy, kde kazdy zanam ma podobu mapy - kazdy zaznam v logu by mel reprezentovat stav
 * distribuovane databaze v danem okamziku.
 *
 * Vasi implementaci budeme testovat v ruznych scenarich (viz evaluation.RaftRun a oficialni
 * zadani). Nasim cilem je, abyste zvladli implementovat jednoduchou distribuovanou key/value
 * databazi s garancemi podle RAFT.
 */
public class ClusterProcess extends RaftProcess<Map<String, String>> {

  // ostatni procesy v clusteru
  private final List<String> otherProcessesInCluster;
  // maximalni spozdeni v siti
  private final int networkDelays;

  private Integer Term;
  private int time;
  private int election_timer;

  private State state;
  private String my_vote;
  private Set<String> votes; // get votes

  private String current_leader;

  public ClusterProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox,
      List<String> otherProcessesInCluster, int networkDelays) {
    super(id, inbox, outbox);
    this.otherProcessesInCluster = otherProcessesInCluster;
    this.networkDelays = max(1, networkDelays);

    this.state = State.FOLLOWER;

    this.Term = 0;
    this.time = 0;
    this.election_timer = this.time + ThreadLocalRandom.current().nextInt(2 * this.networkDelays, 6 * this.networkDelays);

    this.current_leader = null;
    this.my_vote = null;

    this.votes = new HashSet<>();
  }

  @Override
  public Optional<Map<String, String>> getLastSnapshotOfLog() {

    // komentar viz deklarace
    return Optional.empty();
  }

  @Override
  public String getCurrentLeader() { return current_leader; }


  @Override
  public void act() {

    // doimplementuje metodu act() podle RAFT

    // krome vlastnich zprav (vasich trid), dostavate typy zprav z balicku raft.messages s rodicem
    // ClientRequest, tak si je projdete, at vite, co je ucelem a obsahem jednotlivych typu.
    // V pripade ClientRequestWithContent dostavate zpravu typu
    // ClientRequestWithContent<StoreOperationEnums, Pair<String, String>>, kde StoreOperationEnums
    // je operace, kterou mate udelat s obsahem paru Pair<String, String>, kde prvni hodnota
    // paru je klic (nikdy neni prazdny) a druha je hodnota (v pripade pozadavku GET je prazdny)

    // dejte si pozor na jednotlive akce podle RAFT. S klientem komunikujte vyhradne pomoci zprav
    // typu ServerResponse v messages

    // na pozadavky klientu odpovidate zpravami typu ServerResponse viz popis podtypu v messages.
    // !!! V TOMTO PRIPADE JE 'requestId' ROVNO POZADAVKU KLIENTA, NA KTERY ODPOVIDATE !!!

    // dalsi podrobnosti naleznete na strance se zadanim

    this.time += 1;

    //======== START ELECTION =============================
    if (this.state != State.LEADER ) {

      if (this.time > this.election_timer) {
        this.votes.clear();

        this.Term += 1;

        if (this.state == State.FOLLOWER) {
          this.state = State.CANDIDATE;
        }

        this.my_vote = getId();
        this.votes.add(getId());

        for (String p : otherProcessesInCluster) {
          send(p, new RequestVote(this.Term, getId()));
        }

        this.election_timer = this.time + ThreadLocalRandom.current().nextInt(2 * this.networkDelays, 6 * this.networkDelays);
      }
    }

    //========== CHECK ELECTION RESULT ==========================
    if (this.state == State.CANDIDATE) {
      if ((otherProcessesInCluster.size() / 2) < this.votes.size()) {

        this.state = State.LEADER;
        this.current_leader = getId();
        this.election_timer = Integer.MAX_VALUE;
      }
    }

    //============== LEADER ===================================
    if (this.state == State.LEADER) {
      for (String p : otherProcessesInCluster) {
          send(p, new AppendEntry(this.Term, getId()));
      }
    }

    //============== MESSAGES =============================
    for (Message m : this.inbox) {
      if (m instanceof RequestVote) {
        RequestVote requestVote = (RequestVote) m;

        if(this.Term < requestVote.term) {
          this.state = State.FOLLOWER;
          this.Term = requestVote.term;
          if (this.election_timer == Integer.MAX_VALUE || this.election_timer > this.time) {
            this.election_timer = this.time + ThreadLocalRandom.current().nextInt(2 * this.networkDelays, 6 * this.networkDelays);
          }
        }

        if (this.Term == requestVote.term) {
          if (this.my_vote == null) {
            this.my_vote = requestVote.sender;
            this.election_timer = this.time + ThreadLocalRandom.current().nextInt(2 * this.networkDelays, 6 * this.networkDelays);
          }
          send(requestVote.sender, new RequestVoteResponse(this.Term, this.my_vote));
        }


      }
      else if (m instanceof RequestVoteResponse) {
        RequestVoteResponse voteResponse = (RequestVoteResponse) m;

        if(this.Term < voteResponse.term) {
          this.state = State.FOLLOWER;
          this.Term = voteResponse.term;
          if (this.election_timer == Integer.MAX_VALUE || this.election_timer > this.time) {
            this.election_timer = this.time + ThreadLocalRandom.current().nextInt(2 * this.networkDelays, 6 * this.networkDelays);
          }
        }

        String id = getId();
        //System.out.println(id);
        //System.out.println("i am:" + id + " cand:" + voteResponse.candidate_id + ":" + id.equals(voteResponse.candidate_id));
        if (this.state == State.CANDIDATE) {
          if(this.Term == voteResponse.term) {
              this.votes.add(voteResponse.sender);
          }
        }
      }
      else if (m instanceof AppendEntry) {
        AppendEntry appendEntry = (AppendEntry) m;

        if(this.Term < appendEntry.term) {
          this.state = State.FOLLOWER;
          this.Term = appendEntry.term;
          if (this.election_timer == Integer.MAX_VALUE || this.election_timer > this.time) {
            this.election_timer = this.time + ThreadLocalRandom.current().nextInt(2 * this.networkDelays, 6 * this.networkDelays);
          }
        }

        if (this.Term == appendEntry.term) {
          this.state = State.FOLLOWER;
          this.current_leader = appendEntry.sender;
          this.my_vote = null;
          this.election_timer = this.time + ThreadLocalRandom.current().nextInt(2 * this.networkDelays, 6 * this.networkDelays);
        }

        send(appendEntry.sender, new AppendEntryResponse(Term));
      }
      else if (m instanceof AppendEntryResponse) {
        AppendEntryResponse appendEntryResponse = (AppendEntryResponse) m;

        if(this.Term < appendEntryResponse.term) {
          this.state = State.FOLLOWER;
          this.Term = appendEntryResponse.term;
          if (this.election_timer == Integer.MAX_VALUE || this.election_timer > this.time) {
            this.election_timer = this.time + ThreadLocalRandom.current().nextInt(2 * this.networkDelays, 6 * this.networkDelays);
          }
        }
        
      }
    }

    // clear the inbox
    inbox.clear();
  }
}
