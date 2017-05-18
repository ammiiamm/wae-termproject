class ConfigurationsController < ApplicationController
  before_action :authenticate_user!
  load_and_authorize_resource
  before_action :set_configuration, only: [:show, :edit, :update, :destroy]

  # GET /configurations
  # GET /configurations.json
  def index
    @configurations = ::Configuration.all
  end

  # GET /configurations/1
  # GET /configurations/1.json
  def show
  end

  # GET /configurations/new
  def new
    @configuration = ::Configuration.new
  end

  # GET /configurations/1/edit
  def edit
  end

  # POST /configurations
  # POST /configurations.json
  def create
    # @configuration = ::Configuration.new(configuration_params)
    # @configuration.user_id = current_user.id
    # uploaded_io = params[:configuration][:slotconfiguration]
    # File.open(Rails.root.join('public', 'uploads', uploaded_io.original_filename), 'wb') do |file|
    #   file.write(uploaded_io.read)
    # end
    # @configuration.slotconfiguration = uploaded_io.original_filename
    #
    # respond_to do |format|
    #   if @configuration.save
    #     format.html { redirect_to @configuration, notice: 'Configuration was successfully created.' }
    #     format.json { render :show, status: :created, location: @configuration }
    #   else
    #     format.html { render :new }
    #     format.json { render json: @configuration.errors, status: :unprocessable_entity }
    #   end
    # end
    ####
    @configuration = ::Configuration.new(configuration_params)
    @configuration.user_id = current_user.id
    uploaded_io = params[:configuration][:slotconfiguration]
    File.open(Rails.root.join('public', 'uploads', uploaded_io.original_filename), 'wb') do |file|
      file.write(uploaded_io.read)
    end
    @configuration.slotconfiguration = uploaded_io.original_filename

    canCreate = true;
    if User.find(current_user.id).role.name != "admin"
      @permitedSites = Usersite.where(user_id: current_user.id)

      if @permitedSites.nil?
        canCreate = false;
      else
        canCreate = false;
        @permitedSites.each { |permited|
          if permited.site_id == @configuration.site_id
            if permited.add == "1"
              canCreate = true;
            end
          end
        }
      end

    end
    if canCreate
      respond_to do |format|
        if @configuration.save
          format.html { redirect_to @configuration, notice: 'Configuration was successfully created.' }
          format.json { render :show, status: :created, location: @configuration }
        else
          format.html { render :new }
          format.json { render json: @configuration.errors, status: :unprocessable_entity }
        end
      end
    else
      @Alert = "Permission denied. Please contact administrator"
      render :new
    end

  end

  # PATCH/PUT /configurations/1
  # PATCH/PUT /configurations/1.json
  def update
    @configuration.user_id = current_user.id
    respond_to do |format|
      if @configuration.update(configuration_params)
        format.html { redirect_to @configuration, notice: 'Configuration was successfully updated.' }
        format.json { render :show, status: :ok, location: @configuration }
      else
        format.html { render :edit }
        format.json { render json: @configuration.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /configurations/1
  # DELETE /configurations/1.json
  def destroy
    @configuration.destroy
    respond_to do |format|
      format.html { redirect_to configurations_url, notice: 'Configuration was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_configuration
      @configuration = ::Configuration.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def configuration_params
      params.require(:configuration).permit(:name, :config_status, :site_id, :user_id, :camera_id, :zone_id, :slotconfiguration, :roihomo)
    end
end
