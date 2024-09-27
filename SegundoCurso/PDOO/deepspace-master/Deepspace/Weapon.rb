#encoding: UTF-8

require_relative 'WeaponToUI'

module Deepspace
    class Weapon 

        def initialize(name,type,uses)
            @name=name
            @type=type
            @uses=uses
        end


        def name 
            @name
        end

        def type
            @type
        end

        def uses
            @uses
        end

        def power
            @type.power
        end

        

        def self.newCopy (s)
            return new(s.name,s.type,s.uses)
        end


        def useIt

            if uses>0
                output=self.power
                @uses-=1
            
            else output=1.0

            end
            
            return output   

        end

        #Capa que permite conectar el modelo con la interfaz de 
        #usuario manteniendo cierto nivel de aislamiento
        def getUIversion()
            return (WeaponToUI.new(self))
        end  

        #Metodo publico, devuelve una representación en forma
        # de cadena de caracteres de una instancia
        def to_s()
            
            output="\nWeapon attributes:"
            output+= "\nweapon_name=" +name.to_s
            output+= "\nweapon_type=" +type.to_s
            output+= "\nweapon_uses=" +uses.to_s

            return output
            
        end

    end

end
            