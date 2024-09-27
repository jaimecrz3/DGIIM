#encoding: UTF-8

require_relative 'SpaceStation'
require_relative 'PowerEfficientSpaceStationToUI'
require_relative 'Transformation'

module Deepspace

    #Encapsula la mejora de una Space Station a PowerEfficientSpaceStation
    class PowerEfficientSpaceStation < SpaceStation

        #Potencia extra de disparo y proteccion.
        @@EFFICIENCYFACTOR=1.1

        #param: station --> SpaceStation
        def initialize (station)
            newCopy(station)  #Si ponemos super(station) llamaría al initialize de SpaceStation, que recibe otros parametros
        end

        #Interfaz

        def getUIversion
            return (PowerEfficientSpaceStationToUI.new(self))
        end

        def fire()
            @@EFFICIENCYFACTOR*super
        end

        def protection()
            @@EFFICIENCYFACTOR*super
        end

        #Se permite la transformacion a BetaPowerEfficientSpaceStation,
        #pero no a SpaceCity.
    
        def setLoot(loot)
        
            transformation=super(loot)
            
            if(transformation==Transformation::SPACECITY) 
                transformation= Transformation::NOTRANSFORM
            end
            return transformation
        end
        

    end
end