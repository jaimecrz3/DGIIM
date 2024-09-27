/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

import java.util.ArrayList;


public class Hangar {
    
    private int maxElements;
    private ArrayList<Weapon> weapons;
    private ArrayList<ShieldBooster> shieldBoosters;
    
    
    Hangar(int capacity){
        
        maxElements=capacity;
        weapons = new ArrayList<Weapon>();
        shieldBoosters = new ArrayList<ShieldBooster>();
    }
    
    Hangar(Hangar h){
        
        maxElements=h.maxElements;
        weapons = new ArrayList<Weapon>(h.weapons);
        shieldBoosters = new ArrayList<ShieldBooster>(h.shieldBoosters);
    }
    
    //Interfaz
    HangarToUI getUIversion(){return (new HangarToUI(this));}
    
    
    private boolean spaceAvailable(){
        
        boolean available=false;
        if ((weapons.size() + shieldBoosters.size()) < maxElements) available=true;
        
        return available;
    }
    
    
    public boolean addWeapon(Weapon w){
        
        boolean available=false;
        if (spaceAvailable()){
            
            weapons.add(w);
            available=true;
        }
        
        return available;
    }
    
    
    public boolean addShieldBooster(ShieldBooster s){
        
        boolean available=false;
        
        if (spaceAvailable()){
            
            shieldBoosters.add(s);
            available=true;
        }
        
        return available;
    }
    
    
    public int getMaxElements(){return maxElements;}
    
    
    public ArrayList<Weapon> getWeapons(){return weapons;}
    
    public ArrayList<ShieldBooster> getShieldBoosters(){return shieldBoosters;}
    
    
    public Weapon removeWeapon(int w){
        
        Weapon weapon=null;
        
        if (0<=w && w < weapons.size()){
            
            weapon=weapons.get(w);
            weapons.remove(w);
        }
        
        return weapon;
    }
    
    public ShieldBooster removeShieldBooster(int s){
        
        ShieldBooster shieldbooster=null;
        
        if (0<=s && s < shieldBoosters.size()){
            
            shieldbooster=shieldBoosters.get(s);
            shieldBoosters.remove(s);
        }
        
        return shieldbooster;
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
