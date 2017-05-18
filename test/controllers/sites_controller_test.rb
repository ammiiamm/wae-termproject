require 'test_helper'

class SitesControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers

  setup do
    @site = sites(:one)
  end

  test "should get index" do
    sign_in users(:one)
    get :index
    assert_response :success
    assert_not_nil assigns(:sites)
  end

  test "should get new" do
    sign_in users(:one)
    get :new
    assert_response :success
  end

  test "should create site" do
    sign_in users(:one)
    sign_in users(:one)
    assert_difference('Site.count') do
      post :create, site: { address: @site.address, city: @site.city, districs: @site.districs, name: @site.name, postalcode: @site.postalcode, user_id: @site.user_id }
    end

    assert_redirected_to site_path(assigns(:site))
  end

  test "should show site" do
    sign_in users(:one)
    get :show, id: @site
    assert_response :success
  end

  test "should get edit" do
    sign_in users(:one)
    get :edit, id: @site
    assert_response :success
  end

  test "should update site" do
    sign_in users(:one)
    patch :update, id: @site, site: { address: @site.address, city: @site.city, districs: @site.districs, name: @site.name, postalcode: @site.postalcode, user_id: @site.user_id }
    assert_redirected_to site_path(assigns(:site))
  end

  test "should destroy site" do
    # sign_in users(:one)
    # assert_difference('Site.count', -1) do
    #   delete :destroy, id: @site
    # end
    #
    # assert_redirected_to sites_path
  end
end
