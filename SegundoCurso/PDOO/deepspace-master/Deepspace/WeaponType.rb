#encoding: UTF-8

module Deepspace
    module WeaponType

        class Type

            def initialize (power)
                @power=power
            end


            def power
                @power
            end
            
            #Metodo publico, devuelve una representación en forma
            # de cadena de caracteres de una instancia
            def to_s()
                if(power==2.0)
                    output="Laser"
                elsif(power==3.0)
                    output="Missile"
                else
                    output="Plasma"
                end

                return output
            end

        end

        LASER = Type.new(2.0)
        MISSILE = Type.new(3.0)
        PLASMA = Type.new(4.0)


    end

end