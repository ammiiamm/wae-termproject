require 'test_helper'

class AdvertisementsControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers

  setup do
    @advertisement = advertisements(:one)
  end

  test "should get index" do
    sign_in users(:one)
    get :index
    assert_response :success
    assert_not_nil assigns(:advertisements)
  end

  test "should get new" do
    sign_in users(:one)
    get :new
    assert_response :success
  end

  test "should create advertisement" do
    sign_in users(:one)
    assert_difference('Advertisement.count') do
      post :create, advertisement: { add_type: @advertisement.add_type, contecnt: @advertisement.contecnt, site_id: @advertisement.site_id, status: @advertisement.status, user_id: @advertisement.user_id, zone_id: @advertisement.zone_id }
    end

    assert_redirected_to advertisement_path(assigns(:advertisement))
  end

  test "should show advertisement" do
    sign_in users(:one)
    get :show, id: @advertisement
    assert_response :success
  end

  test "should get edit" do
    sign_in users(:one)
    get :edit, id: @advertisement
    assert_response :success
  end

  test "should update advertisement" do
    sign_in users(:one)
    patch :update, id: @advertisement, advertisement: { add_type: @advertisement.add_type, contecnt: @advertisement.contecnt, site_id: @advertisement.site_id, status: @advertisement.status, user_id: @advertisement.user_id, zone_id: @advertisement.zone_id }
    assert_redirected_to advertisement_path(assigns(:advertisement))
  end

  test "should destroy advertisement" do
    sign_in users(:one)
    assert_difference('Advertisement.count', -1) do
      delete :destroy, id: @advertisement
    end

    assert_redirected_to advertisements_path
  end
end
