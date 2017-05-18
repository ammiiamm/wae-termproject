require 'test_helper'

class ParkingslotsControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers

  setup do
    @parkingslot = parkingslots(:one)
  end

  test "should get index" do
    # sign_in users(:one)
    # get :index
    # assert_response :success
    # assert_not_nil assigns(:parkingslots)
  end

  test "should get new" do
    sign_in users(:one)
    get :new
    assert_response :success
  end

  test "should create parkingslot" do
    assert_difference('Parkingslot.count') do
      post :create, parkingslot: { name: @parkingslot.name, point1: @parkingslot.point1, point2: @parkingslot.point2, point3: @parkingslot.point3, point4: @parkingslot.point4, status: @parkingslot.status, zone_id: @parkingslot.zone_id }
    end

    assert_redirected_to parkingslot_path(assigns(:parkingslot))
  end

  test "should show parkingslot" do
    get :show, id: @parkingslot
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @parkingslot
    assert_response :success
  end

  test "should update parkingslot" do
    patch :update, id: @parkingslot, parkingslot: { name: @parkingslot.name, point1: @parkingslot.point1, point2: @parkingslot.point2, point3: @parkingslot.point3, point4: @parkingslot.point4, status: @parkingslot.status, zone_id: @parkingslot.zone_id }
    assert_redirected_to parkingslot_path(assigns(:parkingslot))
  end

  test "should destroy parkingslot" do
    assert_difference('Parkingslot.count', -1) do
      delete :destroy, id: @parkingslot
    end

    assert_redirected_to parkingslots_path
  end
end
