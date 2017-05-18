class ParkingtotalsController < ApplicationController
  before_action :set_parkingtotal, only: [:show, :edit, :update, :destroy]

  # GET /parkingtotals
  # GET /parkingtotals.json
  def index
    @parkingtotals = Parkingtotal.all
  end

  # GET /parkingtotals/1
  # GET /parkingtotals/1.json
  def show
  end

  # GET /parkingtotals/new
  def new
    @parkingtotal = Parkingtotal.new
  end

  # GET /parkingtotals/1/edit
  def edit
  end

  # POST /parkingtotals
  # POST /parkingtotals.json
  def create
    @parkingtotal = Parkingtotal.new(parkingtotal_params)

    respond_to do |format|
      if @parkingtotal.save
        format.html { redirect_to @parkingtotal, notice: 'Parkingtotal was successfully created.' }
        format.json { render :show, status: :created, location: @parkingtotal }
      else
        format.html { render :new }
        format.json { render json: @parkingtotal.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /parkingtotals/1
  # PATCH/PUT /parkingtotals/1.json
  def update
    respond_to do |format|
      if @parkingtotal.update(parkingtotal_params)
        format.html { redirect_to @parkingtotal, notice: 'Parkingtotal was successfully updated.' }
        format.json { render :show, status: :ok, location: @parkingtotal }
      else
        format.html { render :edit }
        format.json { render json: @parkingtotal.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /parkingtotals/1
  # DELETE /parkingtotals/1.json
  def destroy
    @parkingtotal.destroy
    respond_to do |format|
      format.html { redirect_to parkingtotals_url, notice: 'Parkingtotal was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_parkingtotal
      @parkingtotal = Parkingtotal.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def parkingtotal_params
      params.require(:parkingtotal).permit(:zone_id, :free)
    end
end
