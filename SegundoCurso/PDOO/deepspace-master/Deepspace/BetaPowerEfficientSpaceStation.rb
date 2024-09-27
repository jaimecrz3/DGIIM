#encoding: UTF-8

require_relative 'PowerEfficientSpaceStation'
require_relative 'BetaPowerEfficientSpaceStationToUI'
require_relative 'Dice'

module Deepspace

    #Encapsula la transformacion a BetaPowerEfficientSpaceStation
    class BetaPowerEfficientSpaceStation < PowerEfficientSpaceStation

        
        #Potencia extra de disparo y proteccion.
        @@EXTRAEFFICIENCY=1.2

        #param: station --> SpaceStation
        def initialize (station)
            super(station)

            @dice = Dice.new
        end

        #Interfaz

        def getUIversion
            return (BetaPowerEfficientSpaceStationToUI.new(self))
        end

        def fire()
            fire=super
            if(@dice.extraEfficiency()) 
                fire*=@@EXTRAEFFICIENCY
            end

            return fire
        end


    end
end