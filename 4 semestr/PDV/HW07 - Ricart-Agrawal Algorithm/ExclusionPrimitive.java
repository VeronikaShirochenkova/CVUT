package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;
import cz.cvut.fel.agents.pdv.clocked.ClockedProcess;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ExclusionPrimitive {

    /**
     * Stavy, ve kterych se zamek muze nachazet.
     */
    enum AcquisitionState {
        RELEASED,    // Uvolneny   - Proces, ktery vlastni aktualni instanci ExclusionPrimitive o pristup do kriticke
                     //              sekce nezada

        WANTED,      // Chteny     - Proces, ktery vlastni aktualni instanci ExclusionPrimitive zada o pristup do
                     //              kriticke sekce. Ten mu ale zatim nebyl odsouhlasen ostatnimi procesy.

        HELD         // Vlastneny  - Procesu bylo prideleno pravo pristupovat ke sdilenemu zdroji.
    }

    private ClockedProcess owner;            // Proces, ktery vlastni aktualni instanci ExclusionPrimitive

    private String criticalSectionName;      // Nazev kriticke sekce. POZOR! V aplikaci se muze nachazet vice kritickych
                                             // sekci s ruznymi nazvy!

    private String[] allAccessingProcesses;  // Seznam vsech procesu, ktere mohou chtit pristupovat ke kriticke sekci
                                             // s nazvem 'criticalSectionName' (a tak spolurozhoduji o udelovani pristupu)

    private AcquisitionState state;          // Aktualni stav zamku (vzhledem k procesu 'owner').
                                             // state==HELD znamena, ze proces 'owner' muze vstoupit do kriticke sekce

    private int requestTime;
    private boolean getAllOkMessages;
    Map<String, Boolean> AllOkMessages;
    List<String> AllRequestMessages;
    // Doplnte pripadne vlastni datove struktury potrebne pro implementaci
    // Ricart-Agrawalova algoritmu pro vzajemne vylouceni

    public ExclusionPrimitive(ClockedProcess owner, String criticalSectionName, String[] allProcesses) {
        this.owner = owner;
        this.criticalSectionName = criticalSectionName;
        this.allAccessingProcesses = allProcesses;


        // Na zacatku je zamek uvolneny
        this.state = AcquisitionState.RELEASED;
        this.requestTime = 0;
        this.getAllOkMessages = true;
        this.AllOkMessages = new HashMap<>();
        this.AllRequestMessages = new ArrayList<>();

        for(String process : this.allAccessingProcesses) {
            if (!process.equals(this.owner.id)) {
                this.AllOkMessages.put(process, false);
            }
        }
    }

    public int get_id(String name){
        String id = "";
        char[] get_id = name.toCharArray();
        for (char i : get_id) {
            if (i >= '0' && i <= '9') {
                id += i;
            }
        }
        return Integer.parseInt(id);
    }

    /**
     * Metoda pro zpracovani nove prichozi zpravy
     *
     * @param m    prichozi zprava
     * @return 'true', pokud je zprava 'm' relevantni pro aktualni kritickou sekci.
     */
    public boolean accept(ClockedMessage m) {

        // Implementujte zpracovani prijimane zpravy informujici
        // o pristupech ke sdilenemu zdroji podle Ricart-Agrawalova
        // algoritmu. Pokud potrebujete posilat specificke zpravy,
        // vytvorte si pro ne vlastni tridy.
        //
        // POZOR! Ne vsechny zpravy, ktere v teto metode dostanete Vas
        // budou zajimat! Budou Vam prichazet i zpravy, ktere se  napriklad
        // tykaji jinych kritickych sekci. Pokud je zprava nerelevantni, tak
        // ji nezpracovavejte a vratte navratovou hodnotu 'false'. Nekdo jiny
        // se o ni urcite postara :-)
        //
        // Nezapomente se starat o cas procesu 'owner'
        // pomoci metody owner.increaseTime(). Aktualizaci
        // logickeho casu procesu s prijatou zpravou (pomoci maxima) jsme
        // za Vas jiz vyresili.
        //
        // Cas poslani prijate zpravy muzete zjistit dotazem na hodnotu
        // m.sentOn. Aktualni logicky cas muzete zjistit metodou owner.getTime().
        // Zpravy se posilaji pomoci owner.send(prijemce, zprava) a je jim auto-
        // maticky pridelen logicky cas odeslani. Retezec identifikujici proces
        // 'owner' je ulozeny v owner.id.

        // REQUEST MESSAGE
        if (m instanceof RequestMessage) {
            // if the name of critical section in request message isn't equivalent to the current name of the critical section
            if (!((RequestMessage) m).criticalSectionName.equals(this.criticalSectionName)) {
                return false;
            }
            // if critical section is already held
            if (this.state == AcquisitionState.HELD) {
                this.AllRequestMessages.add(m.sender);
            }
            // if critical section is already wanted by owner and owner sent request message earlier than this process
            else if (this.state == AcquisitionState.WANTED && (this.requestTime < m.sentOn ||
                    (this.requestTime == m.sentOn && this.get_id(this.owner.id) < this.get_id(m.sender)))) {
                this.AllRequestMessages.add(m.sender);
            }
            // if critical section is not held/wanted or this process sent request earlier than owner
            else {
                this.owner.send(m.sender, new OkMessage(this.criticalSectionName));
            }
            return true;
        }

        // OK MESSAGE
        if (m instanceof OkMessage) {

            // if the name of critical section in ok message isn't equivalent to the current name of the critical section
            if (!((OkMessage) m).criticalSectionName.equals(this.criticalSectionName)) {
                return false;
            }

            this.getAllOkMessages = true;
            this.AllOkMessages.replace(m.sender, true);
            for (String process : this.allAccessingProcesses) {
                if (!process.equals(this.owner.id)) {
                    if (!this.AllOkMessages.get(process)) {
                        this.getAllOkMessages = false;
                        break;
                    }
                }
            }
            if (this.getAllOkMessages && this.state == AcquisitionState.WANTED) {
                // increase the owner's logical time
                this.owner.increaseTime();
                // change state of critical section
                this.state = AcquisitionState.HELD;
            }
            return true;
        }

        return false;
    }

    public void requestEnter() {

        // Implementujte zadost procesu 'owner' o pristup
        // ke sdilenemu zdroji 'criticalSectionName'

        // get owner's id
        String owner_id = this.owner.id;
        // increase the owner's logical time
        this.owner.increaseTime();
        // change state of critical section
        this.state = AcquisitionState.WANTED;
        // getting the time to send the request message
        this.requestTime = this.owner.getTime();

        // send request message to all other processes
        for (String process : this.allAccessingProcesses) {
            if (!process.equals(owner_id)) {
                this.AllOkMessages.replace(process, false);
                this.owner.send(process, new RequestMessage(this.criticalSectionName));
            }
        }
    }

    public void exit() {

        // Implementuje uvolneni zdroje, aby k nemu meli pristup i
        // ostatni procesy z 'allAccessingProcesses', ktere ke zdroji
        // mohou chtit pristupovat
        // increase the owner's logical time
        this.owner.increaseTime();
        // change state of critical section
        this.state = AcquisitionState.RELEASED;


        // send OK messages to all other processes that have requested access to this critical section
        if (!this.AllRequestMessages.isEmpty()) {
            for (String process : this.AllRequestMessages) {
                this.owner.send(process, new OkMessage(this.criticalSectionName));
            }
            // clear array this processes id
            this.AllRequestMessages.clear();
        }

        // "delete" all received OK messages
        for ( String process : this.allAccessingProcesses) {
            if (!process.equals(this.owner.id)) {
                this.AllOkMessages.replace(process, false);
            }
        }

    }

    public String getName() {
        return criticalSectionName;
    }

    public boolean isHeld() {
        return state == AcquisitionState.HELD;
    }

}
