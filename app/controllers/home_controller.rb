class HomeController < ApplicationController
  before_action :redirect_to_root, :if => :signed_in?
  def show
    @configs = ::Configuration.where(config_status: true)
    @zones = Zone.all
    @parkingtotal = Parkingtotal.all;

  end

  def redirect_to_root
    redirect_to welcome_index_path
  end
end
