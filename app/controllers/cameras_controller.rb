class CamerasController < ApplicationController
  load_and_authorize_resource
  before_action :authenticate_user!
  before_action :set_camera, only: [:show, :edit, :update, :destroy]
  skip_before_filter  :verify_authenticity_token

  # GET /cameras
  # GET /cameras.json
  def index
    @cameras = Camera.all
  end

  # GET /cameras/1
  # GET /cameras/1.json
  def show
  end

  # GET /cameras/new
  def new
    @camera = Camera.new
  end

  # GET /cameras/1/edit
  def edit
  end

  # POST /cameras
  # POST /cameras.json
  def create
    @camera = Camera.new(camera_params)

    respond_to do |format|
      if @camera.save
        format.html { redirect_to @camera, notice: 'Camera was successfully created.' }
        format.json { render :show, status: :created, location: @camera }
      else
        format.html { render :new }
        format.json { render json: @camera.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /cameras/1
  # PATCH/PUT /cameras/1.json
  def update
    respond_to do |format|
      if @camera.update(camera_params)
        format.html { redirect_to @camera, notice: 'Camera was successfully updated.' }
        format.json { render :show, status: :ok, location: @camera }
      else
        format.html { render :edit }
        format.json { render json: @camera.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /cameras/1
  # DELETE /cameras/1.json
  def destroy
    @camera.destroy
    respond_to do |format|
      format.html { redirect_to cameras_url, notice: 'Camera was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  def start
    p "test"
    # context = ZMQ::Context.new
    # requester = context.socket(ZMQ::REQ)
    # requester.connect("tcp://localhost:5555")
    # string = 'S http://10.32.20.201/now.jpg?snap=spush?dummy=0 testcam'
    # requester.send_string string
    render :text => "HELLO"
  end

  def stop
    context = ZMQ::Context.new
    requester = context.socket(ZMQ::REQ)
    requester.connect("tcp://localhost:5555")
    string = 'E http://10.32.20.201/now.jpg?snap=spush?dummy=0 testcam'
    requester.send_string string
    render :text => "HELLO"
  end

  around_action :rescue_from_fk_contraint, only: [:destroy]
  def rescue_from_fk_contraint
    begin
      yield
    rescue ActiveRecord::InvalidForeignKey
      flash[:alert] = "Invalid destroy item. Please contact administrator."
      redirect_to cameras_path
      # Flash and render, render API json error... whatever
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_camera
      @camera = Camera.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def camera_params
      params.require(:camera).permit(:name, :ip_address, :cam_type, :lense, :config, :other_info)
    end
end
