require 'json'
class WelcomeController < ApplicationController
  before_action :authenticate_user!
  def index
    @users = User.all
    @Usercout = @users.count
    @Recently = @users.where(role_id: 1).count
  end
end
