require 'ffi-rzmq'
require 'json'
class DashboardController < ApplicationController
  skip_before_filter  :verify_authenticity_token
  before_action :authenticate_user!
  def show
    if current_user.role.name == "admin" || current_user.role.name == "user" || current_user.role.name == "viewer"
      if ::Configuration.where(config_status: true).first.nil?
        @Alert =  "Please Create a configuration Before going to the dashboard"
        render "welcome/index"
      else
        @configurations = ::Configuration.where(config_status: true)
        @Selected = @configurations.first.id
      end

    else
      @Alert =  "Access denied. You do not have a privileges to access your requested page."
      render "welcome/index"
    end
  end

  def getInfo
     id = params[:id]
     render :text => ::Configuration.find(id).name
  end

  def test
    context = ZMQ::Context.new
    requester = context.socket(ZMQ::REQ)
    requester.connect("tcp://localhost:5555")
    string = 'E http://10.32.20.201/now.jpg?snap=spush?dummy=0 testcam'
    requester.send_string string
    render :text => "HELLO"
  end
end
