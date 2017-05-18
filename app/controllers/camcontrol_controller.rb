require 'ffi-rzmq'
require 'json'
class CamcontrolController < ApplicationController
  def start
    id = params[:id]
    @camera = Camera.find(id)
    p @camera.ip_address
    context = ZMQ::Context.new
    requester = context.socket(ZMQ::REQ)
    requester.connect("tcp://localhost:5555")
    string = 'S ' + @camera.ip_address.to_s + ' '+ @camera.id.to_s
    requester.send_string string
    render :text => "DONE"
  end

  def stop
    id = params[:id]
    @camera = Camera.find(id)
    p @camera.ip_address
    context = ZMQ::Context.new
    requester = context.socket(ZMQ::REQ)
    requester.connect("tcp://localhost:5555")
    string = 'E ' + @camera.ip_address.to_s + ' '+ @camera.id.to_s
    requester.send_string string
    render :text => "DONE"
  end

  def generateimage
    stringGen = params[:string];
    context = ZMQ::Context.new
    requester = context.socket(ZMQ::REQ)
    requester.connect("tcp://localhost:5555")
    requester.send_string stringGen
    render :text => "DONE"
  end
end
