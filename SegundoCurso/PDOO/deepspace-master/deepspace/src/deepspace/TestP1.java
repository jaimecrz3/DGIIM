/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;
import java.util.ArrayList;

//Programa principal
public class TestP1 {
    
    public static void main(String[] args){
        
        //WeaponType
        WeaponType weapon_t1 =WeaponType.LASER, weapon_t2=WeaponType.MISSILE, weapon_t3=WeaponType.PLASMA;
        
        System.out.println("\nWeaponType attributes:"
                +" \nPower:" 
                + "\nLASER= "   + weapon_t1.getPower()
                + "\nMISSILE= " + weapon_t2.getPower()
                + "\nPLASMA= "  + weapon_t3.getPower());
        
        
        //Loot
        int nSupplies=1;
        int nWeapons=2;
        int nShields=3;
        int nHangars=4;
        int nMedals=5;
        Loot loot =new Loot(nSupplies,nWeapons,nShields,nHangars,nMedals);
        System.out.println("\nLoot attributes:"
                + "\nnSupplies="+loot.getNSupplies()
                + "\nnWeapons=" +loot.getNWeapons()
                + "\nnShields=" +loot.getNShields()
                + "\nnHangars=" +loot.getNHangars()
                + "\nnMedals="  +loot.getNMedals());

        
        //Supplies Package
        float ammoPower=1.0f;
        float fuelUnits=2.0f;
        float shieldPower=3.0f;
        SuppliesPackage pkg= new SuppliesPackage(ammoPower,fuelUnits,shieldPower);
        System.out.println("\nSupplies Package attributes:"
                + "\nammoPower="   +pkg.getAmmoPower()
                + "\nfuelUnits="   +pkg.getFuelUnits()
                + "\nshieldPower=" +pkg.getShieldPower());
     
        
        //Shield Booster
        String shield_name = "The Shield";
        float boost=1.0f;
        int shield_uses=2;
        ShieldBooster shield =new ShieldBooster(shield_name,boost,shield_uses);
        System.out.println("\nShield Booster attributes:"
                + "\nboost=" +shield.getBoost()
                + "\nuses="  +shield.getUses());
        
        
        //Weapon
        String weapon_name="The Weapon";
        int weapon_uses=2;
        Weapon weapon = new Weapon(weapon_name,weapon_t1,weapon_uses);
        System.out.println("\nWeapon attributes:"
                + "\nweapon_type=" +weapon.getType()
                + "\nweapon_uses=" +weapon.getUses());
        
        
        //Dice
        Dice dice = new Dice();
        final int REPS=100, NPLAYERS=20;
        float speed = 0.5f;
        
        int iwnh_counter0=0, iwnh_counter1=0;
        int iwnw_counter1=0, iwnw_counter2=0, iwnw_counter3=0;
        int iwns_counter0=0, iwns_counter1=0;
        
        ArrayList<Integer> ws_counter = new ArrayList<Integer>();
        for (int i=0; i<NPLAYERS;i++) 
            ws_counter.add(i,0);
        
        
        int fs_counter0=0, fs_counter1=0;
        int ssm_counter0=0, ssm_counter1=0;
        
        for (int i=0; i<REPS; i++){
            
            //initWithNHangars
            int nhangars=dice.initWithNHangars();
            if (nhangars==0) iwnh_counter0++;
            else iwnh_counter1++;
            
            //initWithNWeapons
            int nweapons = dice.initWithNWeapons();
            if (nweapons==1) iwnw_counter1++;
            else if (nweapons==2) iwnw_counter2++;
            else iwnw_counter3++;
            
            //initWithNShields
            int nshields=dice.initWithNShields();
            if (nshields==0) iwns_counter0++;
            else iwns_counter1++;
            
            //whoStarts
            int player=dice.whoStarts(NPLAYERS);
            int new_value = (ws_counter.get(player))+1;
            ws_counter.set(player,new_value);
            
            //firstShot
            GameCharacter shot= dice.firstShot();
            if (shot==GameCharacter.SPACESTATION) fs_counter0++;
            else fs_counter1++;
            
            //spaceStationMoves
            boolean move= dice.spaceStationMoves(speed);
            if (move) ssm_counter0++;
            else ssm_counter1++;
        }
        
        System.out.println("\nNumber of hangars:\n0: "+ iwnh_counter0 + "\n1: "+ iwnh_counter1
                + "\nNumber of weapons:\n1:" + iwnw_counter1 + "\n2:" + iwnw_counter2 + "\n3:" + iwnw_counter3
                + "\nNumber of shields:\n0:" + iwns_counter0 + "\n1:" + iwnw_counter1);
        
        for (int i=0; i<ws_counter.size();i++) System.out.println("\nPlayer " + i +" starts: " + ws_counter.get(i) + " times.");
        
        System.out.println("\nFirst Shot:\nSpace Station: " + fs_counter0  + "\nEnemy Starship: " + fs_counter1
                +"\n\nSpace Station Moves:\nEscapes: "   + ssm_counter0 + "\nHits: "              + ssm_counter1
                +"\n\n");
        
        }
}
