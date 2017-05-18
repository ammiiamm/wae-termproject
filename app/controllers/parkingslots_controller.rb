class ParkingslotsController < ApplicationController
  before_action :set_parkingslot, only: [:show, :edit, :update, :destroy]
  skip_before_filter  :verify_authenticity_token

  # GET /parkingslots
  # GET /parkingslots.json
  def index
    @parkingslots = Parkingslot.all
  end

  # GET /parkingslots/1
  # GET /parkingslots/1.json
  def show
  end

  # GET /parkingslots/new
  def new
    @parkingslot = Parkingslot.new
  end

  # GET /parkingslots/1/edit
  def edit
  end

  # POST /parkingslots
  # POST /parkingslots.json
  def create
    @parkingslot = Parkingslot.new(parkingslot_params)

    respond_to do |format|
      if @parkingslot.save
        format.html { redirect_to @parkingslot, notice: 'Parkingslot was successfully created.' }
        format.json { render :show, status: :created, location: @parkingslot }
      else
        format.html { render :new }
        format.json { render json: @parkingslot.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /parkingslots/1
  # PATCH/PUT /parkingslots/1.json
  def update
    respond_to do |format|
      if @parkingslot.update(parkingslot_params)
        format.html { redirect_to @parkingslot, notice: 'Parkingslot was successfully updated.' }
        format.json { render :show, status: :ok, location: @parkingslot }
      else
        format.html { render :edit }
        format.json { render json: @parkingslot.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /parkingslots/1
  # DELETE /parkingslots/1.json
  def destroy
    @parkingslot.destroy
    respond_to do |format|
      format.html { redirect_to parkingslots_url, notice: 'Parkingslot was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_parkingslot
      @parkingslot = Parkingslot.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def parkingslot_params
      params.require(:parkingslot).permit(:zone_id, :name, :status, :point1, :point2, :point3, :point4)
    end
end
