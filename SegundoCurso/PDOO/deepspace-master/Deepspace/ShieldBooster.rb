#encoding: UTF-8

require_relative 'ShieldToUI'

module Deepspace

    class ShieldBooster

        def initialize(name,boost,uses)

            @name=name
            @boost=boost
            @uses=uses
        end

        def name 
            @name
        end

        def boost
            @boost
        end

        def uses
            @uses
        end


        def self.newCopy(s)
            return new(s.name,s.boost,s.uses)
        end

        def useIt
            
            if (@uses>0)
                output=@boost
                @uses-=1

            else output=1.0
            
            end

            return output

        end

        #Capa que permite conectar el modelo con la interfaz de 
        #usuario manteniendo cierto nivel de aislamiento
        def getUIversion()
            return (ShieldToUI.new(self))
        end  

        #Metodo publico, devuelve una representación en forma
        # de cadena de caracteres de una instancia
        def to_s()
            
            output="\nShield Booster attributes:"
            output+= "\nboost=" +boost.to_s
            output+= "\nuses="  +uses.to_s

            return output
            
        end

        

    end

end