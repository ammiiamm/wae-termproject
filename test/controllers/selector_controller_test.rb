require 'test_helper'

class SelectorControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers

  test "Random Image Name" do
    sign_in users(:one)
    post :select
    assert_response :success
  end
  test "Random Image without sign_in" do
    post :select
    assert_response 302
  end
  test "Random Image without Admin" do
    sign_in users(:two)
    post :select
    assert_response 200
  end
  # test "Change configuration and start Process" do
  #   sign_in users(:one)
  #   post :change, id: 1, val: 1
  #   assert_response :success
  # end
  # test "Change configuration and stop Process" do
  #   sign_in users(:one)
  #   post :change, id: 1, val: 0
  #   assert_response :success
  # end
end
