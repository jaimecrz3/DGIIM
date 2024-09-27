/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

import java.util.ArrayList;

/**
 *
 * @author leandrofdez
 */

//Encapsula la mejora de una Space Station a SpaceCity.
public class SpaceCity extends SpaceStation {
    
    //Estacion espacial principal de la SpaceCity
    private SpaceStation base;
    
    //Resto de estaciones espaciales que componen la SpaceCity.
    private ArrayList<SpaceStation> collaborators;
    
    
    
    
    public SpaceCity(SpaceStation base, ArrayList<SpaceStation> rest){
        super(base);
        this.base= base;
        this.collaborators= new ArrayList<>(rest);
    }
    
    
    
    //Getters
    public ArrayList<SpaceStation> getCollaborators(){
        
        return this.collaborators;
    }
    
    @Override
    public SpaceCityToUI getUIversion(){return (new SpaceCityToUI(this));}
    
    
    /*
    Se suma la potencia de disparo de todas las estaciones de la Space City.
    */
    @Override
    public float fire(){
        
        float fire= super.fire();
        
        for (int i=0; i<collaborators.size();i++) 
            fire+=collaborators.get(i).fire();
        
        return fire;
        
        
    }
    
    
    /*
    Se suma la potencia de escudo de todas las estaciones de la Space City.
    */
    @Override
    public float protection(){
        
        float protection= super.protection();
        
        for (int i=0; i<collaborators.size();i++) 
            protection+=collaborators.get(i).protection();
        
        return protection;
        
        
    }
    
    /*
    Se acepta un Loot pero se anula la posibilidad de cualquier otra 
    transformacion.
    */
    @Override
    public Transformation setLoot(Loot loot){
        
        super.setLoot(loot);
        return Transformation.NOTRANSFORM;
        
    }
    
}
