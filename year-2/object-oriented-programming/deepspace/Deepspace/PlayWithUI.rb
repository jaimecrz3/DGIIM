
require_relative 'GameUniverse'
require_relative 'TextMainView'
require_relative 'Controller'

class PlayWithUI
  
  def self.main 
    game = Deepspace::GameUniverse.new
    ui = View::TextMainView.instance
    controller = Controller::Controller.instance
    controller.setModelView(game,ui)
    controller.start();
  end
end

PlayWithUI.main
