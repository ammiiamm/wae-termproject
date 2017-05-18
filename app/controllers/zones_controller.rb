class ZonesController < ApplicationController
  before_action :authenticate_user!
  load_and_authorize_resource
  before_action :set_zone, only: [:show, :edit, :update, :destroy]

  # GET /zones
  # GET /zones.json
  def index
    @zones = Zone.all
  end

  # GET /zones/1
  # GET /zones/1.json
  def show
    @users = User.all

  end

  # GET /zones/new
  def new
    @zone = Zone.new
  end

  # GET /zones/1/edit
  def edit
  end

  # POST /zones
  # POST /zones.json
  def create
    @zone = Zone.new(zone_params)
    @zone.user_id = current_user.id
    canCreate = true;
    if User.find(current_user.id).role.name != "admin"
      @permitedSites = Usersite.where(user_id: current_user.id)
      if @permitedSites.nil?
        canCreate = false;
      else
        canCreate = false;
        @permitedSites.each { |permited|
          if permited.site_id == @zone.site_id
            if permited.add == "1"
              canCreate = true;
            end
          end
        }
      end
    end
    if canCreate
      respond_to do |format|
        if @zone.save
          format.html { redirect_to @zone, notice: 'Zone was successfully created.' }
          format.json { render :show, status: :created, location: @zone }
        else
          format.html { render :new }
          format.json { render json: @zone.errors, status: :unprocessable_entity }
        end
      end
    else
      @Alert = "Permission denied. Please contact administrator"
      render :new
    end
  end

  # PATCH/PUT /zones/1
  # PATCH/PUT /zones/1.json
  def update
    @zone.user_id = current_user.id
    respond_to do |format|
      if @zone.update(zone_params)
        format.html { redirect_to @zone, notice: 'Zone was successfully updated.' }
        format.json { render :show, status: :ok, location: @zone }
      else
        format.html { render :edit }
        format.json { render json: @zone.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /zones/1
  # DELETE /zones/1.json
  def destroy
    @zone.destroy
    respond_to do |format|
      format.html { redirect_to zones_url, notice: 'Zone was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  around_action :rescue_from_fk_contraint, only: [:destroy]

  def rescue_from_fk_contraint
    begin
      yield
    rescue ActiveRecord::InvalidForeignKey
      flash[:alert] = "Invalid destroy item. Please contact administrator."
      redirect_to zones_path
      # Flash and render, render API json error... whatever
    end
  end

  private
  # Use callbacks to share common setup or constraints between actions.
  def set_zone
    @zone = Zone.find(params[:id])
  end

  # Never trust parameters from the scary internet, only allow the white list through.
  def zone_params
    params.require(:zone).permit(:name, :site_id, :numberofslots, :numberfree, :user_id, :Long, :Lat)
  end


end
