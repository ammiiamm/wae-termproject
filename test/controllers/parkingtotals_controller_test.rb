require 'test_helper'

class ParkingtotalsControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers

  setup do
    @parkingtotal = parkingtotals(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:parkingtotals)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create parkingtotal" do
    assert_difference('Parkingtotal.count') do
      post :create, parkingtotal: { free: @parkingtotal.free, zone_id: @parkingtotal.zone_id }
    end

    assert_redirected_to parkingtotal_path(assigns(:parkingtotal))
  end

  test "should show parkingtotal" do
    get :show, id: @parkingtotal
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @parkingtotal
    assert_response :success
  end

  test "should update parkingtotal" do
    patch :update, id: @parkingtotal, parkingtotal: { free: @parkingtotal.free, zone_id: @parkingtotal.zone_id }
    assert_redirected_to parkingtotal_path(assigns(:parkingtotal))
  end

  test "should destroy parkingtotal" do
    assert_difference('Parkingtotal.count', -1) do
      delete :destroy, id: @parkingtotal
    end

    assert_redirected_to parkingtotals_path
  end
end
