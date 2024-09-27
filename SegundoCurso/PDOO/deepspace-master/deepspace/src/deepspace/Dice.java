/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;
import java.util.Random;

//Tiene la responsabilidad de tomar las decisiones que dependen del azar del juego.

class Dice {
    
    private final float NHANGARSPROB;
    private final float NSHIELDSPROB;
    private final float NWEAPONSPROB;
    private final float FIRSTSHOTPROB;
    private final float EXTRAEFFICIENCYPROB;
    private Random generator;
    
    Dice(){
        
        NHANGARSPROB=0.25f;
        NSHIELDSPROB=0.25f;
        NWEAPONSPROB=0.33f;
        FIRSTSHOTPROB=0.5f;
        EXTRAEFFICIENCYPROB=0.8f;
        generator=new Random();
    }
    
    public int initWithNHangars(){
        
        int output;
        float prob= generator.nextFloat();
        
        if (prob < NHANGARSPROB) output=0;
        else output=1;
        //output=1;
        
        return output;
    }
    
    public int initWithNWeapons(){
        
        int output;
        float prob= generator.nextFloat();
        
        if (prob < NWEAPONSPROB) output=1;
        else if (prob >=NWEAPONSPROB && prob < 2*NWEAPONSPROB) output=2;
        else output=3;
        
        return output;
        
    }
    
    public int initWithNShields(){
        
        int output;
        float prob= generator.nextFloat();
        
        if (prob < NSHIELDSPROB) output=0;
        else output=1;
        
        return output;
    }
    
    int whoStarts(int nPlayers){
        
        return generator.nextInt(nPlayers);
    }
    
    public GameCharacter firstShot(){
        
        GameCharacter output;
        float prob= (generator.nextFloat());
        
        if (prob < FIRSTSHOTPROB) output=GameCharacter.SPACESTATION;
        else output=GameCharacter.ENEMYSTARSHIP;
        
        return output;
    }
    
    public boolean spaceStationMoves(float speed){
        
        boolean output;
        float prob= generator.nextFloat();
        
        if (prob < speed) output=true;
        else output=false;
        
        return output;
    }
    
    //---------------------------------------------------
    //Practica4
    
    public boolean extraEfficiency(){
        
        boolean output;
        float prob= generator.nextFloat();
        
        if(prob < EXTRAEFFICIENCYPROB){
            output=true;
        } else{
            output=false;
        }
        
        return output;
        
    }
    
    public String toString(){
        
        String output= "Dice attributes:\n"      +
                       "NHANGARS probability: "  + NHANGARSPROB         + "\n" +
                       "NSHIELD probability: "   + NSHIELDSPROB         + "\n" +
                       "NWEAPONS probability: "  + NWEAPONSPROB         + "\n" +
                       "FIRSTSHOT probability: " + FIRSTSHOTPROB        + "\n" +
                       "Generator: "             + generator.toString() + "\n";
        
        return output;
    }
    
}
