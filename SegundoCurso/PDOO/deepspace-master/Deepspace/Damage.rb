#encoding: UTF-8

require_relative 'DamageToUI'

module Deepspace

    #Representa el daño producido a una estacion espacial cuando se pierde
    #el combate. 
    class Damage 
        
        #Constructor: Recibe el numero de escudos
        def initialize (s)
            @nShields=s 
        end

        #Hacemos privado el new ya que Ruby no implementa clases abstractas
        private_class_method :new

        #Metodo para adjustar los escudos, es comun tanto a NumericDamage y SpecficDamage
        def adjust(s)
           return ([@nShields, s.size].min)
        end
           
        #Decrementa en una unidad el número de potenciadores de escudo que deben ser 
        #eliminados.
        #Ese contador no puede ser inferior a cero en ningún caso
        def discardShieldBooster
            if (@nShields>0)
                @nShields-=1
            end
        end

        def nShields
            @nShields
        end


    end
end
