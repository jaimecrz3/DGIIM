/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

import java.util.ArrayList;
/**
* Hereda de la clase abstracta Damage
* Versión con tipos de armas de Damage
*/
public class SpecificDamage extends Damage {
    
    //Atributos
    //Lista con los tipos de armas que se van a perder.
    private ArrayList<WeaponType> weapons;
    
    //Constante para indicar desuso.
    private static final int NOUSE=-1;
    
    //Constructor
    SpecificDamage(ArrayList<WeaponType> wl, int s){
        super(s);
        
        if(wl!=null){
            weapons=new ArrayList<>(wl);
        }else{
            weapons=new ArrayList<>();
        }
    }
    
    @Override
    public Damage copy(){
        return (new SpecificDamage(this.weapons,this.getNShields()));
    }
    
    //Consultores
    
    public ArrayList<WeaponType> getWeapons(){return weapons;}
    
    
    //Interfaz
    @Override
    public SpecificDamageToUI getUIversion(){return (new SpecificDamageToUI(this));}
    
    private int arrayContainsType(ArrayList<Weapon> w, WeaponType t){
        
        int index=NOUSE;
        boolean seguir=true;
        for (int i=0; i<w.size() && seguir; i++){
            if (w.get(i).getType()==t){
                
                index=i;
                seguir=false;
            }
        }
        
        return index;
        
    }
    
    //Redefinimos Adjust
    @Override
    public SpecificDamage adjust(ArrayList<Weapon> w, ArrayList<ShieldBooster> s){
        
        int shields_copia;
        //Ajustamos los escudos
        if(getNShields()>s.size())
            shields_copia=s.size();
        else
            shields_copia=getNShields();  //Se usa el del padre
        
        ArrayList<WeaponType> weapons_copia=new ArrayList<>();
        ArrayList<Weapon> w_aux=new ArrayList<>(w);
        /*
        Si un tipo de arma esta en w_aux la añadimos a weapons_copia,
        si no esta no la añadimos ya que entonces perderiamos tipos de armas
        que no estan en w
        */
        for(int i=0; i<weapons.size(); i++){

            int indice=arrayContainsType(w_aux, weapons.get(i));
            //Si el tipo de weapon esta en el array w_aux la añadimos a las
            //armas que se pueden perder(las de damage)
            if(indice!=-1){  
                weapons_copia.add(weapons.get(i));
                w_aux.remove(indice);  //Lo borramos porque ya hemos encontrado este tipo de arma
            }

        }

        return (new SpecificDamage(weapons_copia, shields_copia));
        
    }
    
    //Redifinimos hasNoEffect
    @Override
    public boolean hasNoEffect(){
        
        return (getNShields()==0 && weapons.isEmpty());
    
    }
    
    @Override
    public void discardWeapon(Weapon w){
        
        int index= weapons.indexOf(w.getType());
        if (index>=0) weapons.remove(index); 
        
    }
    
    @Override
    public String toString(){
        
        String weapons_string= "WeaponsType: ";
        for(int i=0; i<weapons.size(); i++){
            weapons_string += weapons.get(i).toString() + " ";
        }
        
        return ("SpecificDamage:\n" + "nShields: "+ getNShields()+ "\n" + 
                   weapons_string + "\n");
    }
    
    
    
    
}
