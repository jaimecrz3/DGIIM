/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

/**
 *
 * @author leandrofdez
 */

//Encapsula la mejora de una Space Station a PowerEfficientSpaceStation.
class PowerEfficientSpaceStation extends SpaceStation{
    
    //Potencia extra de disparo y proteccion.
    private static final float EFFICIENCYFACTOR=1.1f;
    
    
    
    public PowerEfficientSpaceStation(SpaceStation station){
         super(station);
        
    }

    
    
    /*
    Añade un factor extra a la potencia disparo.
    */
    @Override
    public float fire(){
        
        return EFFICIENCYFACTOR*super.fire();
    }
    
    /*
    Añade un factor extra a la potencia de escudo.
    */
    @Override
    public float protection(){
        
        return EFFICIENCYFACTOR*super.protection();
    }
    
    
    /*
    Se permite la transformacion a BetaPowerEfficientSpaceStation,
    pero no a SpaceCity.
    */
    @Override
    public Transformation setLoot(Loot loot){
        
        Transformation transformation= super.setLoot(loot);
        
        if(transformation==Transformation.SPACECITY) 
            transformation= Transformation.NOTRANSFORM;
        
        return transformation;
    }
    
    //Interfaz
    @Override
    public PowerEfficientSpaceStationToUI getUIversion(){
        return (new PowerEfficientSpaceStationToUI(this));
    }
    
    
}
