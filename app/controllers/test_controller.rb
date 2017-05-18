require 'google_maps_service'

class TestController < ApplicationController
  def index
    @data = nil
    gmaps = GoogleMapsService::Client.new(key: 'AIzaSyAyxD49sJePDuiXFWSMxlykH-z6x-Ki5qg')
  end
end
