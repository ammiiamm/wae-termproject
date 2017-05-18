require 'test_helper'

class ZonesControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers

  setup do
    @zone = zones(:one)
  end

  test "should get index" do
    sign_in users(:one)
    get :index
    assert_response :success
    assert_not_nil assigns(:zones)
  end

  test "should get new" do
    sign_in users(:one)
    get :new
    assert_response :success
  end

  test "should create zone" do
    sign_in users(:one)
    assert_difference('Zone.count') do
      post :create, zone: { name: @zone.name, numberfree: @zone.numberfree, numberofslots: @zone.numberofslots, site_id: @zone.site_id, user_id: @zone.user_id }
    end

    assert_redirected_to zone_path(assigns(:zone))
  end

  test "should show zone" do
    sign_in users(:one)
    get :show, id: @zone
    assert_response :success
  end

  test "should get edit" do
    sign_in users(:one)
    get :edit, id: @zone
    assert_response :success
  end

  test "should update zone" do
    sign_in users(:one)
    patch :update, id: @zone, zone: { name: @zone.name, numberfree: @zone.numberfree, numberofslots: @zone.numberofslots, site_id: @zone.site_id, user_id: @zone.user_id }
    assert_redirected_to zone_path(assigns(:zone))
  end

  test "should destroy zone" do
    # sign_in users(:one)
    # assert_difference('Zone.count', -1) do
    #   delete :destroy, id: @zone
    # end
    #
    # assert_redirected_to zones_path
  end
end
