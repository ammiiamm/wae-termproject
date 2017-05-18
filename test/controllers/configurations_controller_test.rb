require 'test_helper'

class ConfigurationsControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers

  setup do
    sign_in users(:one)
    @configuration = configurations(:one)

  end


  test "should get new" do
    sign_in users(:one)
    get :new
    assert_response :success
  end

  test "should create configuration Admin" do
    sign_in users(:one)
    test_image = "/home/theekshana/Documents/MainWebApp/app/assets/images/files.jpg"
    file = Rack::Test::UploadedFile.new(test_image, "image/jpeg")
    assert_difference('Configuration.count') do
      post :create, configuration: { camera_id: @configuration.camera_id, config_status: @configuration.config_status, name: @configuration.name, roihomo: @configuration.roihomo, site_id: @configuration.site_id, slotconfiguration: file, user_id: @configuration.user_id, zone_id: @configuration.zone_id }
    end

    assert_redirected_to configuration_path(assigns(:configuration))
  end



  test "should show configuration" do
    sign_in users(:one)
    get :show, id: @configuration
    assert_response :success
  end

  test "should get edit" do
    sign_in users(:one)
    get :edit, id: @configuration
    assert_response :success
  end

  test "should update configuration" do
    patch :update, id: @configuration, configuration: { camera_id: @configuration.camera_id, config_status: @configuration.config_status, name: @configuration.name, roihomo: @configuration.roihomo, site_id: @configuration.site_id, slotconfiguration: @configuration.slotconfiguration, user_id: @configuration.user_id, zone_id: @configuration.zone_id }
    assert_redirected_to configuration_path(assigns(:configuration))
  end

  test "should destroy configuration" do
    sign_in users(:one)
    assert_difference('Configuration.count', -1) do
      delete :destroy, id: @configuration
    end

    assert_redirected_to configurations_path
  end

  test "should create configuration User Permited by Admin" do
    sign_in users(:two)
    test_image = "app/assets/images/cplogo.jpg"

    file = Rack::Test::UploadedFile.new(test_image, "image/jpeg")
    assert_difference('Configuration.count') do
      post :create, configuration: { camera_id: @configuration.camera_id, config_status: @configuration.config_status, name: @configuration.name, roihomo: @configuration.roihomo, site_id: @configuration.site_id, slotconfiguration: file, user_id: @configuration.user_id, zone_id: @configuration.zone_id }
    end

    assert_redirected_to configuration_path(assigns(:configuration))
  end
end
