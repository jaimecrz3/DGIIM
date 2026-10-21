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
public class SpaceStationToUI {
    private String name;
    private int nMedals;
    
    private float ammoPower=1.0f;
    private float fuelUnits=1.0f;
    private float shieldPower=1.0f;
    
    private ArrayList<WeaponToUI> weapons;
    private ArrayList<ShieldToUI> shieldBoosters;
    private HangarToUI hangar;   
    private DamageToUI pendingDamage;
    
    SpaceStationToUI(SpaceStation station) {
        weapons=new ArrayList<>();
        shieldBoosters=new ArrayList<>();         
        
        name=station.getName();
        nMedals=station.getNMedals();
        
        ammoPower=station.getAmmoPower();
        fuelUnits=station.getFuelUnits();
        shieldPower=station.getShieldPower();
        
        for (Weapon w:station.getWeapons()) {
            weapons.add(w.getUIversion());
        }
        
        for(ShieldBooster s:station.getShieldBoosters()) {
            shieldBoosters.add(s.getUIversion());
        }
        
        Hangar h=station.getHangar();
        if (h!=null) {
            hangar=h.getUIversion();
        }
        else {
            hangar=null;
        }
        
        // MIGUEL: Añadido de  pendingDamage  y su consultor
        
        Damage d = station.getPendingDamage();
        if (d != null) {
          pendingDamage = d.getUIversion();
        } else {
          pendingDamage = null;
        }
            
    }

    public String getName() {
        return name;
    }

    public int getnMedals() {
        return nMedals;
    }

    public float getAmmoPower() {
        return ammoPower;
    }

    public float getFuelUnits() {
        return fuelUnits;
    }

    public float getShieldPower() {
        return shieldPower;
    }

    public ArrayList<WeaponToUI> getWeapons() {
        return weapons;
    }

    public ArrayList<ShieldToUI> getShieldBoosters() {
        return shieldBoosters;
    }

    public HangarToUI getHangar() {
        return hangar;
    }
    
    public DamageToUI getPendingDamage() {
        return pendingDamage;
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
        
        
        
        String output= "ammoPower: "        + ammoPower           + "\n" +
                       "fuelUnits: "        + fuelUnits           + "\n" +
                        "name: "            + name          + "\n" +
                        "nMedals: "        + nMedals           + "\n" +
                        "shieldPower: "        + shieldPower          + "\n" +
                        weapons_string + "\n" +
                        shieldBoosters_string + "\n" +
                        hangar.toString() + pendingDamage.toString();
        
        return output;
    }
}
