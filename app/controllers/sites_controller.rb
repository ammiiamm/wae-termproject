require 'google_maps_service'
class SitesController < ApplicationController
  before_action :authenticate_user!
  load_and_authorize_resource
  before_action :set_site, only: [:show, :edit, :update, :destroy]
  # GET /sites
  # GET /sites.json
  def index
    @sites = Site.all
  end

  # GET /sites/1
  # GET /sites/1.json
  def show
  end

  # GET /sites/new
  def new
    @site = Site.new
    gmaps = GoogleMapsService::Client.new(key: 'AIzaSyAyxD49sJePDuiXFWSMxlykH-z6x-Ki5qg')
    results = gmaps.geocode('1600 Amphitheatre Parkway, Mountain View, CA')
  end

  # GET /sites/1/edit
  def edit
  end

  # POST /sites
  # POST /sites.json
  def create
    @site = Site.new(site_params)
    @site.user_id = current_user.id
    respond_to do |format|
      if @site.save
        format.html { redirect_to @site, notice: 'Site was successfully created.' }
        format.json { render :show, status: :created, location: @site }
      else
        format.html { render :new }
        format.json { render json: @site.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /sites/1
  # PATCH/PUT /sites/1.json
  def update
    @site.user_id = current_user.id
    respond_to do |format|
      if @site.update(site_params)
        format.html { redirect_to @site, notice: 'Site was successfully updated.' }
        format.json { render :show, status: :ok, location: @site }
      else
        format.html { render :edit }
        format.json { render json: @site.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /sites/1
  # DELETE /sites/1.json
  def destroy
    @site.destroy
    respond_to do |format|
      format.html { redirect_to sites_url, notice: 'Site was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  around_action :rescue_from_fk_contraint, only: [:destroy]
  def rescue_from_fk_contraint
    begin
      yield
    rescue ActiveRecord::InvalidForeignKey
      flash[:alert] = "Invalid destroy item. Please contact administrator."
      redirect_to sites_path
      # Flash and render, render API json error... whatever
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_site
      @site = Site.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def site_params
      params.require(:site).permit(:name, :address, :districs, :city, :postalcode, :user_id, :Long, :Lat)
    end
end
