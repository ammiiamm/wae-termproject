require 'test_helper'

class TestControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers

  test "should get index" do
    get :index
    assert_response :success
  end

end
