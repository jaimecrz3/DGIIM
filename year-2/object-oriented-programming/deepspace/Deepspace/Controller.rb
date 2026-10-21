
require 'singleton'

module Controller
  
    DS=Deepspace


  class Controller
    
    include Singleton
    
    @@WEAPON = 0x1
    @@SHIELD = 0x2
    @@HANGAR = 0x4
    
    def self.WEAPON
      @@WEAPON
    end
    
    def self.SHIELD
      @@SHIELD
    end
    
    def self.HANGAR
      @@HANGAR
    end
    
    attr_reader :model, :view
    
    def initialize ()
      @model = nil
      @view = nil
    end
    
    public

    def setModelView (aModel, aView)
      @model = aModel
      @view = aView
    end
  
    def start() 
        @model.init(@view.readNamePlayers())
        @view.updateView()
        @view.showView()
    end
    
    def finish(i)
      if @view.confirmExitMessage()
        exit(i)
      end
    end

    def nextTurn() 
        nextTurnAllowed = @model.nextTurn()
        if !nextTurnAllowed
          @view.nextTurnNotAllowedMessage()
        end
        return nextTurnAllowed
    end

    def combat() 
        result = @model.combat()
        case result
        when DS::CombatResult::ENEMYWINS
          @view.lostCombatMessage()
        when DS::CombatResult::STATIONESCAPES
          @view.escapeMessage()
        when DS::CombatResult::STATIONWINS
          @view.wonCombatMessage()
          if @model.haveAWinner()
            @view.wonGameMessage()
            exit(0)
          end
        when DS::CombatResult::NOCOMBAT
          @view.noCombatMessage()
        when DS::CombatResult::STATIONWINSANDCONVERTS
          @view.conversionMessage()
          if @model.haveAWinner()
            @view.wonGameMessage()
            exit(0)
          end
        end
    end

    def getUIversion() 
        return @model.getUIversion()
    end

    def getState() 
        return @model.state()
    end
    
    private
    
    def invertArray (array)
      array.reverse!
    end

    public
    
    def discardHangar() 
        @model.discardHangar()
        @view.updateView()
    end

    def mount (weapons, shields) 
      invertArray (weapons);
      invertArray (shields);
    
      for i in weapons
        @model.mountWeapon(i)
      end
      
      for i in shields
        @model.mountShieldBooster(i)
      end
    end
    
    def discard (places, weapons, shields) 
      invertArray(weapons);
      invertArray(shields);
    
      if ((places & Controller.WEAPON) == Controller.WEAPON) 
        for i in weapons
          @model.discardWeapon(i)
        end
      elsif ((places & Controller.SHIELD) == Controller.SHIELD)
        for i in shields
          @model.discardShieldBooster(i)
        end
      elsif ((places & Controller.HANGAR) == Controller.HANGAR)
        for i in weapons
          @model.discardWeaponInHangar(i)
        end
        for i in shields
          @model.discardShieldBoosterInHangar(i)
        end
      end
    end
      
  end # class

end # module
