/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

import java.util.ArrayList;


//Esta clase representa el daño que puede recibir una estacion espacial.

public abstract class Damage {
    
    
    //ClaseAbstracta
    
    //Parámetros
    //Indica la perdida de un numero determinado de escudos.
    private int nShields;
    
    //Constructor
    Damage (int s){
        nShields=s;
    }
    
    /*
        Método de instancia copy que devolverá una copia del objeto que reciba el mensaje. 
    Cuando sea necesario crear una copia de un objeto que represente daños, 
    simplemente se llamará a este método copy de la instancia que se desea copiar.
    */
    public abstract Damage copy();
    
     //Interfaz
    public abstract DamageToUI getUIversion();
    
    /*
    Devuelve una versión ajustada del objeto a w y s.
    Se devuelve una copia del mismo pero reducida si es necesario para que no 
    implique perder armas o potenciadores de escudos que no están en las
    colecciones de los parámetros.
    */
    public abstract Damage adjust(ArrayList<Weapon> w, ArrayList<ShieldBooster> s);
    
    /*
    Decrementa en una unidad el número de potenciadores de escudo que deben ser 
    eliminados.
    
    Ese contador no puede ser inferior a cero en ningún caso.
    */
    public void discardShieldBooster(){if (nShields>0)nShields--;}
    
    /*
    
    Si la instancia dispone de una lista de tipos concretos de armas, intenta 
    eliminar el tipo del arma pasada como parámetro de esa lista. 
    En otro caso simplemente decrementa en una unidad el contador de armas que 
    deben ser eliminadas. 
    
    Ese contador no puede ser inferior a cero en ningún caso.
    */
    
    public abstract void discardWeapon(Weapon w);
    
     /*
    Devuelve true si el daño no implica la pérdida de ningún tipo de accesorio 
    (armas o potenciadores de escudo).
    */
    public abstract boolean hasNoEffect();
    
    //Consultor
    public int getNShields(){return nShields;}
    
    public abstract String toString();
    
}
    
    //---------------------------------------------------------
    
    //Indica la perdida de un numero determinado de armas.
    /*private int nWeapons;
    
    //Indica la perdida de un numero determinado de escudos.
    private int nShields;
    
    //Lista con los tipos de armas que se van a perder.
    private ArrayList<WeaponType> weapons;
    
    //Constante para indicar desuso.
    private static final int NOUSE=-1;
    
    
    //Constructores
    Damage (int w, int s){
        
        nWeapons=w;
        nShields=s;
        weapons=null;
    }
    
    Damage (ArrayList<WeaponType> wl, int s){
        
        nWeapons= NOUSE;
        nShields=s;
        weapons=new ArrayList<WeaponType>(wl);
    }
    
    Damage (Damage d){
        
        nWeapons=d.nWeapons;
        nShields=d.nShields;
        weapons=new ArrayList<WeaponType>(d.weapons);  //Deben ser instancias independientes
    }
    
    
    //Interfaz
    DamageToUI getUIversion(){return (new DamageToUI(this));}
    
    
    //Others
    
    /*Devuelve el indice de la primera arma de w, cuyo tipo coincida con t. Si
    no se encuentra devuelve -1.
    
    
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
    
    
    /*
    Devuelve una versión ajustada del objeto a w y s.
    Se devuelve una copia del mismo pero reducida si es necesario para que no 
    implique perder armas o potenciadores de escudos que no están en las
    colecciones de los parámetros.
    
    
    public Damage adjust(ArrayList<Weapon> w, ArrayList<ShieldBooster> s){
        
        int shields_copia;
        //Ajustamos los escudos
        if(nShields>s.size())
            shields_copia=s.size();
        else
            shields_copia=nShields;
        
        //Ajustamos las armas
        if(nWeapons == NOUSE){     //Trabajamos con el vector
            ArrayList<WeaponType> weapons_copia=new ArrayList<>();
            ArrayList<Weapon> w_aux=new ArrayList<Weapon>(w);
            /*
            Si un tipo de arma esta en w_aux la añadimos a weapons_copia,
            si no esta no la añadimos ya que entonces perderiamos tipos de armas
            que no estan en w
            */
    /*        for(int i=0; i<weapons.size(); i++){
                
                int indice=arrayContainsType(w_aux, weapons.get(i));
                //Si el tipo de weapon esta en el array w_aux la añadimos a las
                //armas que se pueden perder(las de damage)
                if(indice!=-1){  
                    weapons_copia.add(weapons.get(i));
                    w_aux.remove(indice);  //Lo borramos porque ya hemos encontrado este tipo de arma
                }
                
            }
            
            return (new Damage(weapons_copia, shields_copia));
            
        } else{
            int weapons_copia;
            if(nWeapons>w.size())
                weapons_copia=w.size();
            else
                weapons_copia=nWeapons;
            return (new Damage(weapons_copia, shields_copia));
        }
        
    }
    
    
    /*
    
    Si la instancia dispone de una lista de tipos concretos de armas, intenta 
    eliminar el tipo del arma pasada como parámetro de esa lista. 
    En otro caso simplemente decrementa en una unidad el contador de armas que 
    deben ser eliminadas. 
    
    Ese contador no puede ser inferior a cero en ningún caso.
    
    
    public void discardWeapon(Weapon w){
        
        if (weapons!=null) {
            int index= weapons.indexOf(w.getType());
            if (index>=0) weapons.remove(index); 
        }
            
            
        else if (nWeapons>0)nWeapons--;
    }
    
    
    /*
    Decrementa en una unidad el número de potenciadores de escudo que deben ser 
    eliminados.
    
    Ese contador no puede ser inferior a cero en ningún caso.
    
    
    public void discardShieldBooster(){if (nShields>0)nShields--;}
        
    
    /*
    Devuelve true si el daño no implica la pérdida de ningún tipo de accesorio 
    (armas o potenciadores de escudo).
    
    
    public boolean hasNoEffect(){
        
        boolean output;
        
        if (nWeapons==NOUSE) output = (weapons.size() == 0 && nShields==0);
        else output=(nWeapons==0 && nShields==0);
        
        return output;
    
    }
    
    
    //Getters
    
    public int getNWeapons(){return nWeapons;}
    
    public ArrayList<WeaponType> getWeapons(){return weapons;}
    
    public int getNShields(){return nShields;}
    
    public String toString(){
        
        String weapons_string= "WeaponsType: ";
        for(int i=0; i<weapons.size(); i++){
            weapons_string += weapons.get(i).toString() + " ";
        }
        
        
        
        String output= "nWeapons: "        + nWeapons           + "\n" +
                       "nShields: "        + nShields           + "\n" +
                       weapons_string + "\n";
        
        return output;
    }
    
    
    
}
*/