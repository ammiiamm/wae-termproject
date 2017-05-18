require 'test_helper'

class CamerasControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers  #
  setup do
    @camera = cameras(:one)
  end

  test "should get index" do
    sign_in users(:one)
    get :index
    assert_response :success
    assert_not_nil assigns(:cameras)
  end

  test "should get new" do
    sign_in users(:one)
    get :new
    assert_response :success
  end

  test "should create camera" do
    # assert_difference('Camera.count') do
    #   post :create, camera: { cam_type: @camera.cam_type, config: @camera.config, ip_address: @camera.ip_address, lense: @camera.lense, name: @camera.name, other_info: @camera.other_info }
    # end
    #
    # assert_redirected_to camera_path(assigns(:camera))
  end

  test "should show camera" do
    sign_in users(:one)
    get :show, id: @camera
    assert_response :success
  end

  test "should get edit" do
    sign_in users(:one)
    get :edit, id: @camera
    assert_response :success
  end

  test "should update camera" do
    sign_in users(:one)
    patch :update, id: @camera, camera: { cam_type: @camera.cam_type, config: @camera.config, ip_address: @camera.ip_address, lense: @camera.lense, name: @camera.name, other_info: @camera.other_info }
    assert_redirected_to camera_path(assigns(:camera))
  end

  test "should destroy camera" do
    # sign_in users(:one)
    # assert_difference('Camera.count', -1) do
    #   delete :destroy, id: @camera
    # end
    #
    # assert_redirected_to cameras_path
  end
end
