#encoding:utf-8

require 'singleton'

require_relative 'Controller'
require_relative 'Command'

module View
  
  class Value
    attr_reader :text
    def initialize(aText)
      @text = aText
    end
  end # class Option
  
  module Element
    WEAPON = Value.new("Arma")
    SHIELD = Value.new("Escudo")
    HANGAR = Value.new("Hangar")
  end # Element
  
  module Operation
    MOUNT = Value.new("Montar")
    DISCARD = Value.new("Descartar")
  end # Operation
  
  DS=Deepspace
  CT=Controller
  
class TextMainView
  
  include Singleton
  
  @@mainSeparator = "\n ******* ******* ******* ******* ******* ******* ******* \n"
  @@separator = "\n ------- ------- ------- ------- ------- ------- ------- \n"
  
      
  def initialize()
    @gameUI = nil
    @state = nil
  end
  
  private
  
  def pause(s) 
    print @@mainSeparator
    print @@mainSeparator
    puts s
    print @@mainSeparator
    print @@mainSeparator
    print "\n(pulsa  ENTER  para continuar) "
    gets
  end

  def processCommand(command) 
    case command
      when Command::EXIT 
        CT::Controller.instance.finish(0)
      when Command::SHOWSTATION 
        puts showStation(@gameUI.currentStation)
      when Command::SHOWENEMY
        puts showEnemy(@gameUI.currentEnemy)
      when Command::MOUNTWEAPONS 
        mountDiscardFromHangar(Operation::MOUNT, Element::WEAPON)
      when Command::MOUNTSHIELDS 
        mountDiscardFromHangar(Operation::MOUNT, Element::SHIELD)
      when Command::DISCARDWEAPONSINHANGAR 
        mountDiscardFromHangar(Operation::DISCARD, Element::WEAPON)
      when Command::DISCARDSHIELDSINHANGAR 
        mountDiscardFromHangar(Operation::DISCARD, Element::SHIELD)
      when Command::DISCARDHANGAR 
        CT::Controller.instance.discardHangar()
        pause("\n ******* Hangar Completo Descartado ******* ")
      when Command::DISCARDWEAPONS 
        discardMountedElements(Element::WEAPON)
      when Command::DISCARDSHIELDS 
        discardMountedElements(Element::SHIELD)
      when Command::COMBAT 
        puts "Combatiendo"
        CT::Controller.instance.combat()
      when Command::NEXTTURN 
        CT::Controller.instance.nextTurn()
    end
  end
  
  def readInt (message, min, max) 
    number = -1
    begin
      valid = true
      print message
      input = gets.chomp
      begin  
        number = Integer(input)
        if (number<min || number>max)  # No es un entero entre los válidos
          puts "\nEl numero debe estar entre #{min} y #{max}\n"
          valid = false
        end
      rescue Exception => e # No se ha introducido un entero
        puts "\nDebes introducir un numero.\n"
        valid = false;  
      end
      if !valid
        puts "\n\nInténtalo de nuevo.\n\n"
      end
    end while !valid
    return number
  end
  
  public
  
  def updateView()
    @gameUI = CT::Controller.instance.getUIversion()
    @state = CT::Controller.instance.getState()
  end
  
  def showView() 
    while true # Hasta que se elija en el menú  Salir
      updateView()
      command = Command::EXIT;
      case @state
        when DS::GameState::INIT
          command = getCommandInit()
        when DS::GameState::BEFORECOMBAT 
          command = getCommandBeforeCombat()
        when DS::GameState::AFTERCOMBAT 
          command = getCommandAfterCombat()
      end
      processCommand (command)
    end
  end
  
  def readNamePlayers() 
    names = Array.new
    nPlayers = readInt("\n¿Cuántos jugadores participan (2-4)? ",2,4)
    for i in 0...nPlayers
      print "Escribe el nombre del jugador #{i+1}: "
      names.push(gets.chomp)
    end
    return names
  end
  
  def confirmExitMessage() 
    print "¿Estás segur@ que deseas salir [s/N]? "
    fullInput = gets.chomp
    if !fullInput.empty?
      input = fullInput[0]
      if input == 's' || input == 'S'
        return true
      end
    end
    return false;
  end
  
  private
  
  def manageMenu(message, menu) 
    menuCheck = Hash.new   # Para comprobar que se hace una selección válida

    for c in menu do
      menuCheck [c.menu] = c
    end
    begin # Until a valid selection
      validInput = true
      option = Command::GOBACK.menu;
      puts @@separator
      puts "**** " + message + " ****\n"
      for c in menu do 
        puts '%3d' % [c.menu] + " : " + c.text + "\n"
      end
      print "\n Elige una opción: "
      capture = gets.chomp
      begin
        option = Integer(capture)
        if(! menuCheck.has_key?(option)) then # It's not a valid integer
          validInput = false;
        end
      rescue Exception => e
        validInput = false;
      end
      if(!validInput) then
        inputErrorMessage()
      end
    end while(! validInput)
    return(menuCheck[option])    
  end
  
  def mountDiscardFromHangar(operation, element) 
      option = Command::GOBACK.menu
      elements = Array.new
      noElements = Array.new
      
      begin    # Choice and mount weapons or shields until go back
        howMany = showHangarToMountDiscard(operation, element)
        option = getChoice(howMany)
        elements.clear
        if option != Command::ERRORINPUT.menu
          elements.push(option)
        end
        case element
        when Element::WEAPON
          if operation == Operation::MOUNT
            CT::Controller.instance.mount(elements,noElements)
          else 
            CT::Controller.instance.discard(CT::Controller.HANGAR,elements,noElements)
          end
        when Element::SHIELD
          if operation == Operation::MOUNT
            CT::Controller.instance.mount(noElements,elements)
          else 
            CT::Controller.instance.discard(CT::Controller.HANGAR,noElements,elements)
          end
        end
        updateView()
      end while(option != Command::GOBACK.menu)
  end
  
  public 
  
  def nextTurnNotAllowedMessage() 
    puts "\n No puedes avanzar de turno, no has cumplido tu castigo"
  end
  
  private
  
  def getChoice(howMany) 
    validInput = true
    option = Command::GOBACK.menu
    print("\n Elige: ")
    capture = gets.chomp
    begin
      option = Integer(capture)
      if(option < Command::GOBACK.menu || option > howMany) then  # no se ha escrito un entero en el rango permitido
        validInput = false
      end
    rescue Exception => e   # no se ha escrito un entero
      validInput = false;
    end
    if(! validInput) then
      inputErrorMessage()
      return Command::ERRORINPUT.menu
    end
    return option;
  end
  
  def getCommandInit() 
    commands = [Command::SHOWSTATION, Command::MOUNTWEAPONS, Command::MOUNTSHIELDS, Command::COMBAT, Command::EXIT]
    return manageMenu("Bienvenido  " + @gameUI.currentStation.name + \
             ",  es tu primera vez.\n Organiza tu Armamento para el Combate.\n --- Opciones disponibles", \
             commands)
  end
  
  def getCommandBeforeCombat() 
    commands = [Command::SHOWSTATION, Command::COMBAT, Command::EXIT]
    return manageMenu(@gameUI.currentStation.name + \
            ",  estás en un punto de no retorno.\n Solo te queda Combatir.", commands)
  end

  def getCommandAfterCombat()
    commands = [ Command::SHOWSTATION, 
          Command::MOUNTWEAPONS, Command::MOUNTSHIELDS, 
          Command::DISCARDWEAPONS, Command::DISCARDSHIELDS,
          Command::DISCARDWEAPONSINHANGAR, Command::DISCARDSHIELDSINHANGAR,
          Command::DISCARDHANGAR, 
          Command::SHOWENEMY, Command::NEXTTURN, Command::EXIT ]
      
      return manageMenu(@gameUI.currentStation.name + \
              ",  puedes Reorganizar tu Armamento antes de pasar de turno.\n Opciones disponibles", \
              commands)
  end
  
  def inputErrorMessage()
    puts "\n\n ¡¡¡ E R R O R !!! \n\n Selección errónea. Inténtalo de nuevo.\n\n"
  end
  
  public 
  
  def lostCombatMessage() 
    puts "Has PERDIDO el combate. \tCumple tu castigo."
  end
  
  private 
  
  def discardMountedElements(element) 
      howMany = 0
      option = Command::GOBACK.menu
      elements = Array.new
      noElements = Array.new
      
      begin   # Choice and discard weapons or shields until go back
        puts @@separator
        puts "Elige un " + element.text + " para Descartar"
        puts "\n" + format("%3d",Command::GOBACK.menu) + " : " + Command::GOBACK.text + "\n"
        case element
            when Element::WEAPON 
                puts showWeapons(@gameUI.currentStation.weapons, true)
                howMany = @gameUI.currentStation.weapons.size();
            when Element::SHIELD 
                puts showShields(@gameUI.currentStation.shieldBoosters, true)
                howMany = @gameUI.currentStation.shieldBoosters.size()
        end
        option = getChoice(howMany)
        elements.clear()
        if (option != Command::ERRORINPUT.menu)
          elements.push(option)
        end
        case element
        when Element::WEAPON 
          CT::Controller.instance.discard(CT::Controller.WEAPON, elements, noElements)
        when Element::SHIELD 
          CT::Controller.instance.discard(CT::Controller.SHIELD, noElements, elements)
        end
        updateView()
    end while(option != Command::GOBACK.menu)
  end
  
  public
  
  def escapeMessage()
    puts "Has logrado escapar. \tEres una Gallina Espacial."
  end

  def wonCombatMessage()
    puts "Has GANADO el combate. \tDisfruta de tu botín."
  end
  
  def wonGameMessage() 
    puts "\n\tHAS GANADO LA PARTIDA"
  end

  def conversionMessage()
    puts "Has GANADO el combate. \nAdemás te has CONVERTIDO. \nDisfruta de tu botín"
  end
    
  def noCombatMessage()
    puts "No puedes combatir en este momento"
  end
  
  def showStation(station) 
    out = ""

    out += @@mainSeparator + "\n"
    out += " ***** Información de la  Estación Espacial Actual *****\n"
    out += "       -------------------------------------------\n"
    out += "Nombre ............ : " + station.name + "\n"
    out += "Potencia de fuego . : " + station.ammoPower.to_s + "\n"
    out += "Potencia de defensa : " + station.shieldPower.to_s + "\n"
    out += "Medallas .......... : " + station.nMedals.to_s + "\n"
    out += "Armas montadas : \n"
    tmp = showWeapons(station.weapons, false)
    if("".eql?(tmp)) then
        out += "   Ninguna \n"
    else
        out += tmp
    end
    out += "Potenciadores de Escudos montados : \n"
    tmp = showShields(station.shieldBoosters(), false)
    if("".eql?(tmp))
        out += "   Ninguno \n"
    else
        out += tmp
    end
    out += showHangar(station.hangar)
    out += "Castigo pendiente : "
    out += showDamage(station.pendingDamage)
    return out
  end
  
  def showWeapons(someWeapons, menu) 
    out = "";

    i = 0;
    for aWeapon in someWeapons do
      out += showWeapon(aWeapon,(menu ?(format("%3d",i) + " : ") : " - "))
      i+=1
    end
    return out
  end
  
  def showWeapon(aWeapon, tab) 
    return(tab + aWeapon.type.to_s + " - Potencia: " + aWeapon.power.to_s + " - Usos: " + aWeapon.uses.to_s + "\n")
  end

  def showShields(someShields, menu) 
    out = ""
    
    i = 0
    for aShield in someShields do
      out += showShield(aShield,(menu ?('%3d' % [i]) + " : " : " - "))
      i+=1
    end
    return out
  end
  
  def showShield(aShield, tab) 
      return(tab + "Escudo - Potencia: " + aShield.boost.to_s + " - Usos: " + aShield.uses.to_s + "\n");
  end
  
  def showHangar(aHangar) 
    String out = "";
    if(aHangar != nil) then
        slots = aHangar.maxElements
        out += "Dispone de un Hangar con " + slots.to_s
        out +=(slots == 1 ? " lugar \n" : " lugares \n")
        out += showWeapons(aHangar.weapons, false)
        out += showShields(aHangar.shieldBoosters, false)
    else
      out = "No tiene ningún Hangar\n"
    end
    return out
  end
  
  def showEnemy(anEnemy) 
    out = ""
    out += @@separator + "\n"
    out += " *** Información del Enemigo actual ***\n"
    out += "     ------------------------------\n"
    out += "Nombre ............ : " + anEnemy.name + "\n"
    out += "Potencia de fuego . : " + anEnemy.ammoPower.to_s + "\n"
    out += "Potencia de defensa : " + anEnemy.shieldPower.to_s + "\n"
    out += "Botín : \n"
    out += showLoot(anEnemy.loot)
    out += "Pérdidas : \n"
    out += showDamage(anEnemy.damage)
    return out
  end
  
  def showLoot(aLoot) 
      out = ""
      out += " - Armas ..... : " + aLoot.nWeapons.to_s + "\n"
      out += " - Escudos ... : " + aLoot.nShields.to_s + "\n"
      out += " - Hangares .. : " + aLoot.nHangars.to_s + "\n"
      out += " - Suministros : " + aLoot.nSupplies.to_s + "\n"
      out += " - Medallas .. : " + aLoot.nMedals.to_s + "\n"
      return out
  end
  
  def showDamage(aDamage)
      if (aDamage != nil) then
        out = "\n"
        out += " - Armas . : " + aDamage.getWeaponInfo() + "\n"
        out += " - Escudos : " + aDamage.nShields.to_s + "\n"
      else
        out = "Ninguno\n"
      end
      return out
  end
  
  def showHangarToMountDiscard(operation, element) 
      option = Command::GOBACK.menu
      
      puts @@separator
      puts "Elige un " + element.text + " para " + operation.text
      puts "\n" + format("%3d",Command::GOBACK.menu) + " : " + Command::GOBACK.text + "\n"
      hangar = @gameUI.currentStation.hangar
      if (hangar != nil) then
        case(element) 
            when Element::WEAPON 
                for weapon in hangar.weapons do
                    option+=1
                    print showWeapon(weapon, format("%3d",option) + " : ")
                end
        when Element::SHIELD 
                for shield in hangar.shieldBoosters do
                    option+=1
                    print showShield(shield, format("%3d",option) + " : ")
                end
        end
      end
      return option
  end

  end # class
  
end # module
