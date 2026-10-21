/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

/**
 *
 * @author leandrofdez
 */

//Encapsula la transformacion a BetaPowerEfficientSpaceStation. 
public class BetaPowerEfficientSpaceStation extends PowerEfficientSpaceStation{
    
    
    //Potencia extra de disparo.
    private static final float EXTRAEFFICIENCY=1.2f;
    
    //Dado que nos permitira determinar si se dispara con mas potencia.
    Dice dice;
    
    BetaPowerEfficientSpaceStation(SpaceStation station){
        super(station);
        dice=new Dice();
        
    }
    
    
    /*
    Añade un factor extra a la potencia disparo.
    */
    @Override
    public float fire(){
        
        float fire=super.fire();
        if (dice.extraEfficiency()) fire*=EXTRAEFFICIENCY;
        
        return fire;
    }
    
    //Interfaz
    @Override
    public BetaPowerEfficientSpaceStationToUI getUIversion(){
        return (new BetaPowerEfficientSpaceStationToUI(this));
    }
    
}
