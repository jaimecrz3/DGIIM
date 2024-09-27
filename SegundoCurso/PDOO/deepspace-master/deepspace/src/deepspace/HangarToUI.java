/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package deepspace;

import java.util.ArrayList;

/**
 *
 * @author Profe
 */
public class HangarToUI {
    private int maxElements;
    private ArrayList<WeaponToUI> weapons;
    private ArrayList<ShieldToUI> shieldBoosters;    
    
    HangarToUI(Hangar h) {
        weapons=new ArrayList<>();
        shieldBoosters=new ArrayList<>(); 
        
        maxElements=h.getMaxElements();
        
        for (Weapon w:h.getWeapons()) {
            weapons.add(w.getUIversion());
        }
        
        for(ShieldBooster s:h.getShieldBoosters()) {
            shieldBoosters.add(s.getUIversion());
        }
    }

    public int getMaxElements() {
        return maxElements;
    }

    public ArrayList<WeaponToUI> getWeapons() {
        return weapons;
    }

    public ArrayList<ShieldToUI> getShieldBoosters() {
        return shieldBoosters;
    }
    
    public String toString(){
        
        String weapons_string= "Weapons: ";
        for(int i=0; i<weapons.size(); i++){
            weapons_string += weapons.get(i).toString() + " ";
        }
        
        String shieldBoosters_string= "shieldBoosters: ";
        for(int i=0; i<shieldBoosters.size(); i++){
            weapons_string += shieldBoosters.get(i).toString() + " ";
        }
        
        
        String output= "maxElements: "        + maxElements            + "\n" +
                       weapons_string + "\n" +
                       shieldBoosters_string  + "\n";
        
        return output;
    }
    
}
