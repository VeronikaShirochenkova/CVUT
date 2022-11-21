package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;

import java.sql.SQLOutput;
import java.util.*;
import java.util.Map.Entry;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

/**
 * Trida s implementaci metody act() pro proces Failure Detector. Tuto tridu (a tridy pouzivanych zprav) budete
 * odevzdavat. Do tridy si muzete doplnit vlastni pomocne datove struktury. Hodnoty muzete inicializovat primo
 * v konstruktoru. Klicova je metoda act(), kterou vola kazda instance tridy FailureDetectorProcess ve sve metode
 * act(). Tuto metodu naimplementujte podle protokolu SWIM predstaveneho na prednasce.
 *
 * Pokud si stale jeste nevite rady s frameworkem, inspiraci muzete nalezt v resenych prikladech ze cviceni.
 */
public class ActStrategy {

    // maximalni zpozdeni zprav
    private final int maxDelayForMessages;
    private final List<String> otherProcesses;
    private String processID;
    private int wakeCount;

    Map<String, Integer> PingTimes;

    List <String> maybeDead;

    Map<String, List<String>> PingRequestProcesses;

    Set<String> deadProcesses = new HashSet<>();



    // Definujte vsechny sve promenne a datove struktury, ktere budete potrebovat

    public ActStrategy(int maxDelayForMessages, List<String> otherProcesses,
                       int timeToDetectKilledProcess, int upperBoundOnMessages) {
        this.maxDelayForMessages = maxDelayForMessages;
        this.otherProcesses = otherProcesses;
        this.processID = "unknown";
        this.wakeCount = 0;
        this.PingTimes = new HashMap<>();
        this.PingRequestProcesses = new HashMap<>();
        this.maybeDead = new ArrayList<>();

        // Doplne inicializaci
    }


    /**
     * Metoda je volana s kazdym zavolanim metody act v FailureDetectorProcess. Metodu implementujte
     * tak, jako byste implementovali metodu act() v FailureDetectorProcess, misto pouzivani send()
     * pridejte zpravy v podobe paru - prijemce, zprava do listu. Zpravy budou nasledne odeslany.
     * <p>
     * Diky zavedeni teto metody muzeme kontrolovat pocet odeslanych zprav vasi implementaci.
     */
    public List<Pair<String, Message>> act(Queue<Message> inbox, String disseminationProcess) {

        // Od DisseminationProcess muzete dostat zpravu typu DeadProcessMessage, ktera Vas
        // informuje o spravne detekovanem ukoncenem procesu.
        // DisseminationProcess muzete poslat zpravu o detekovanem "mrtvem" procesu.
        // Zprava musi byt typu PFDMessage.
        List<Pair<String, Message>> outbox = new ArrayList<>();

        while(!inbox.isEmpty()) {
            Message message = inbox.poll();
            // Get personal process ID
            if (this.processID.equals("unknown")) {
                this.processID = message.recipient;
            }

            if (message instanceof PingMessage) {
                outbox.add(new Pair<>(((PingMessage)message).sender, new AckMessage(this.processID)));
            }

            else if (message instanceof PingRequestMessage) {
                String ID = ((PingRequestMessage)message).getProcessID();
                outbox.add(new Pair<>(ID, new PingMessage(ID)));

                if (!this.PingRequestProcesses.containsKey(ID)) {
                    this.PingRequestProcesses.put(ID, new ArrayList<>());
                }
                this.PingRequestProcesses.get(ID).add(((PingRequestMessage)message).sender);

            }

            else if (message instanceof AckMessage) {

                String process = ((AckMessage)message).sender;
                String p = ((AckMessage)message).getProcessID();
                this.PingTimes.remove(process);
                this.PingTimes.remove(p);

                if (this.PingRequestProcesses.containsKey(process)){

                    for (String i : this.PingRequestProcesses.get(process)) {
                        outbox.add(new Pair<>(i, new AckMessage(process)));
//                        System.out.println(this.processID + " send Ack to " + i);
                    }
                    this.PingRequestProcesses.remove(process);
                }

                if (this.maybeDead.contains(process)) {
                    this.maybeDead.remove(process);
                }
            }

            else if (message instanceof DeadProcessMessage) {
                this.otherProcesses.remove(((DeadProcessMessage)message).getProcessID());
            }

        }

        Random random = new Random();

        if(this.wakeCount % 2 == 1) {
            // Ping random neighbour
            int count = 0;
            while (count < 10) {
                int idx = random.nextInt(this.otherProcesses.size());
                String randomProcess = this.otherProcesses.get(idx);

                if (!this.PingTimes.containsKey(randomProcess)) {
                    outbox.add(new Pair<>(randomProcess, new PingMessage(this.processID)));
                    this.PingTimes.put(randomProcess, this.wakeCount);
                    break;
                }
                count++;
            }
        }

        List <String> processRemove = new ArrayList<>();
        List <String> processAdd = new ArrayList<>();
        for(Entry<String, Integer> i : this.PingTimes.entrySet()) {
            if ((this.wakeCount - i.getValue()) > 5 * this.maxDelayForMessages) {
                outbox.add(new Pair<>(disseminationProcess, new PFDMessage(i.getKey())));

                processRemove.add(i.getKey());
            }
            else if ((this.wakeCount - i.getValue()) > 2 * this.maxDelayForMessages && !this.maybeDead.contains(i.getKey())) {
                this.maybeDead.add(i.getKey());
                // random 10 process
                List<String> processes = this.otherProcesses;
                Collections.shuffle(processes);
                for(int j = 0; j < 10; j++) {
                    if (!this.PingTimes.containsKey(processes.get(j))) {
                        outbox.add(new Pair<>(processes.get(j), new PingRequestMessage(i.getKey())));
                        if (!this.PingRequestProcesses.containsKey(i.getKey())) {
                            this.PingRequestProcesses.put(i.getKey(), new ArrayList<>());
                        }
                        this.PingRequestProcesses.get(i.getKey()).add(processes.get(j));
                    }
                }
            }
        }

        for(int k = 0; k < processRemove.size(); k++) {
            this.PingTimes.remove(processRemove.get(k));
            this.PingRequestProcesses.remove(processRemove.get(k));
            this.otherProcesses.remove(processRemove.get(k));
            this.maybeDead.remove(processRemove.get(k));
        }


        outbox = outbox.stream().distinct().collect(Collectors.toList());
        this.wakeCount++;
        return outbox;
    }


}
