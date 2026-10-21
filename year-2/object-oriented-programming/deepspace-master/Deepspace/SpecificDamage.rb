#encoding: UTF-8

require_relative 'Damage'
require_relative 'SpecificDamageToUI'

module Deepspace
    
    #Versión con número de armas de 
    #Hereda de Damage
    class SpecificDamage < Damage

        @NOUSE=-1     #Atributo de instancia(porque estoy en ámbito de clase) de clase( necesitamos un consultor para acceder a el en un metodo de instancia)
                        #otra opcion es hacerlo de clase(dos @@)

        def initialize(wl, s)
            super(s)

            @weapons=wl
        end

        #Necesitamos un metodo de clase para acceder al altributo de instancia
        def self.NOUSE    
            @NOUSE
        end

        #Hacemos publico el metodo new
        public_class_method :new

        #Método de instancia copy que devolverá una copia del objeto que reciba el mensaje. 
        #Cuando sea necesario crear una copia de un objeto que represente daños, 
        #simplemente se llamará a este método copy de la instancia que se desea copiar.
        def copy 
            return (SpecificDamage.new(@weapons, nShields()))
        end

        def weapons
            @weapons
        end

        #Capa que permite conectar el modelo con la interfaz de 
        #usuario manteniendo cierto nivel de aislamiento
        def getUIversion()
            return (SpecificDamageToUI.new(self))
        end  


        #Devuelve el indice de la primera arma de w, cuyo tipo coincida con t.
        # Si no se encuentra devuelve -1
        def arrayContainsType(w,t)
            index=SpecificDamage.NOUSE   #Si pusiesemos index=@NOUSE NOUSE seria otra variable distinta, en este caso de instancia
            i=0
            seguir=true
            while i<w.size() && seguir
                if(w.at(i).type==t)
                    index=i
                    seguir=false  #Para salirnos del bucle
                end
                i += 1
                
            end        
            
               
            return index
        end

        #Devuelve una versión ajustada del objeto a w y s.
        #Se devuelve una copia del mismo pero reducida si es necesario para que no 
        #implique perder armas o potenciadores de escudos que no están en las
        #colecciones de los parámetros.

        #@param w Array de weapons.
        #@param s Array de shieldBoosters
        def adjust(w, s)
            
        
            #Ajustamos los escudos
            #if(@nShields>s.size)
            #    shields_copia=s.size()
            #else
            #    shields_copia=@nShields
            #end

            shields_copia = super(s)

            #Ajustamos las armas
            
            weapons_copia=Array.new()      #Array donde vamos a almacenar la solucion
            w_aux=Array.new(w)
            

            #Si un tipo de arma esta en w_aux la añadimos a weapons_copia,
            #si no esta no la añadimos ya que entonces perderiamos tipos de armas
            #que no estan en w
        

            @weapons.each do |element|
                indice=arrayContainsType(w_aux, element)
                #Si el tipo de weapon esta en el array w_aux la añadimos a las
                #armas que se pueden perder(las de damage)
                if(indice!=SpecificDamage.NOUSE)
                    
                    weapons_copia.push(element)
                    w_aux.delete_at(indice)  #Lo borramos porque ya hemos encontrado este tipo de arma
                end
            end
            return SpecificDamage.new(weapons_copia, shields_copia)
            
        end

        #Devuelve true si el daño no implica la pérdida de ningún tipo de accesorio 
        #(armas o potenciadores de escudo).
        def hasNoEffect()
            
            return (@weapons.size == 0 && nShields()==0)

        end

        #Disminuye, si es posible el numero de armas
        def discardWeapon(w)
        
            i= @weapons.index(w.type)     #Devuelve el primer indice que tiene el objeto(el ultimo con rindex)
            if (i!=nil) 
                #Devuelve el elemento eliminado.
                @weapons.delete_at(i)
            end
        end

        def to_s()
        
            weapons_string= "Weapons: "
            for i in 0..(@weapons.size-1) do
                weapons_string += @weapons.at(i).to_s() + "\n"
            end

            "SpecificDamage:\n" + "nShields: "+ nShields()+ "\n" + weapons_string + "\n"
        end





    end
end