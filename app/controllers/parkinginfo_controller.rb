class ParkinginfoController < ApplicationController
  skip_before_filter  :verify_authenticity_token
  def totalfree
    total = 0;
    id = params[:id]
    @parkingslot = Parkingslot.where(zone_id: id)
    @parkingslot.each do |slot|
      if slot.status == false
        total = total + 1;
      end
    end
    render :text => total
  end
  def free
    id = params[:id]
    @parkingtotal = Parkingtotal.where(zone_id: id).last(10)
    render :json => @parkingtotal
  end

  def addslot
    zone_id = params[:zone_id];
    name = params[:name];
    status = params[:status];
    point1 = params[:point1];
    point2 = params[:point2];
    point3 = params[:point3];
    point4 = params[:point4];
    @newSlot = Parkingslot.new;
    @newSlot.zone_id = zone_id
    @newSlot.name = name
    @newSlot.status = status
    @newSlot.point1 = point1
    @newSlot.point2 = point2
    @newSlot.point3 = point3
    @newSlot.point4 = point4
    @newSlot.save
    render :text => "done"
  end
end
