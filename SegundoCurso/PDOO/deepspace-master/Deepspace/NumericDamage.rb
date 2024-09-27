#encoding: UTF-8

require_relative 'Damage'
require_relative 'NumericDamageToUI'

module Deepspace
    
    #Versión con número de armas de 
    #Hereda de Damage
    class NumericDamage < Damage

        def initialize(w, s)
            super(s)

            @nWeapons=w
        end

        #Hacemos publico el metodo new
        public_class_method :new

        #Método de instancia copy que devolverá una copia del objeto que reciba el mensaje. 
        #Cuando sea necesario crear una copia de un objeto que represente daños, 
        #simplemente se llamará a este método copy de la instancia que se desea copiar.
        def copy 
            return (NumericDamage.new(@nWeapons, nShields()))
        end

        def nWeapons
            @nWeapons
        end

        #Capa que permite conectar el modelo con la interfaz de 
        #usuario manteniendo cierto nivel de aislamiento
        def getUIversion()
            return (NumericDamageToUI.new(self))
        end  


        #Devuelve una versión ajustada del objeto a w y s.
        #Se devuelve una copia del mismo pero reducida si es necesario para que no 
        #implique perder armas o potenciadores de escudos que no están en las
        #colecciones de los parámetros.

        #@param w Array de weapons.
        #@param s Array de shieldBoosters
        def adjust(w, s)
            
        
            #Ajustamos los escudos

            shields_copia = super(s)  #Llama al del padre

            #Ajustamos las armas
            
            weapons_copia= [@nWeapons, w.size].min
            return NumericDamage.new(weapons_copia, shields_copia)
            
        end

        #Devuelve true si el daño no implica la pérdida de ningún tipo de accesorio 
        #(armas o potenciadores de escudo).
        def hasNoEffect()
            
            return (@nWeapons==0 && nShields()==0)

        end

        #Disminuye, si es posible el numero de armas
        def discardWeapon(w)
        
            if (@nWeapons>0)
                @nWeapons-=1
            end
        end

        def to_s()
        
            "NumericDamage:\n" + "nShields: "+ nShields()+ "\n" + "nWeapons: " + nWeapons+ "\n"
        end





    end
end