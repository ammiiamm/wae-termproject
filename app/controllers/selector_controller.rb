require 'securerandom'
require 'ffi-rzmq'
require 'json'
class SelectorController < ApplicationController
  before_action :authenticate_user!

  def select
    @zone = Zone.all
    if current_user.role.name == "admin" || current_user.role.name == "user"
      @cameras = Camera.all
      @configs = ::Configuration.all
      @random_string = SecureRandom.urlsafe_base64(5)
    else
      @Alert =  "Access denied. You do not have a privileges to access your requested page."
      render "welcome/index"
    end

  end
  def addtoconf

  end
  def change
    val = params[:val]
    id = params[:id]
    @config = ::Configuration.find(id);
    zoneid = @config.zone_id;
    camip = @config.camera.ip_address;
    @config.config_status = val;
    @config.save

    if val == '0'
      context = ZMQ::Context.new
      requester = context.socket(ZMQ::REQ)
      requester.connect("tcp://localhost:5556")
      string = 'E ' + id.to_s + ' ' + zoneid.to_s + ' ' + camip.to_s;
      requester.send_string string
    end
    if val == '1'
      context = ZMQ::Context.new
      requester = context.socket(ZMQ::REQ)
      requester.connect("tcp://localhost:5556")
      string = 'S ' + id.to_s + ' ' + zoneid.to_s + ' ' + camip.to_s;
      requester.send_string string
    end
    render :text => "DONE"
  end
end
