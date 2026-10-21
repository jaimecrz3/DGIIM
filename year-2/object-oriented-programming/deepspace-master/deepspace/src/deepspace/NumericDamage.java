/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

import java.util.ArrayList;

/**
 * Hereda de la clase abstracta Damage
 * Versión con número de armas de Damage
 */
public class NumericDamage extends Damage {
    
    //Atributos
    //Indica la perdida de un numero determinado de armas.
    private int nWeapons;
    
    //Constructor
    NumericDamage(int w, int s){
        super (s);
        
        nWeapons=w;
    }
    
    @Override
    public Damage copy(){
        return (new NumericDamage(this.nWeapons,this.getNShields()));
    }
    
    //Consultores
    
    public int getNWeapons(){return (nWeapons);}
    
    
    
    //Interfaz
    @Override
    public NumericDamageToUI getUIversion(){return (new NumericDamageToUI(this));}
    
    @Override
    public NumericDamage adjust(ArrayList<Weapon> w, ArrayList<ShieldBooster> s){
        
        int shields_copia;
        //Ajustamos los escudos
        if(getNShields()>s.size())
            shields_copia=s.size();
        else
            shields_copia=getNShields(); //Llama al metodo del padre
        
        int weapons_copia;
        if(nWeapons>w.size())
            weapons_copia=w.size();
        else
            weapons_copia=nWeapons;
        
        return (new NumericDamage(weapons_copia, shields_copia));
        
    }
    
    @Override
    public boolean hasNoEffect(){
        
        return (nWeapons==0 && getNShields()==0);
        
    }
    
    @Override
    public void discardWeapon(Weapon w){
        
        if (nWeapons>0)nWeapons--;
    }
    
    @Override
    public String toString(){
        
        return ("NumericDamage:\n" + "nShields: "+ getNShields()+ "\n" + 
                     "nWeapons: " + nWeapons+ "\n");
    }
    
}
