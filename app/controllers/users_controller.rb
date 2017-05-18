class UsersController < ApplicationController
  before_action :authenticate_user!
  load_and_authorize_resource
  def status
    #@users = User.all
    @users = User.order('username ASC')
    @usersite = Usersite.all
    @site = Site.all
  end
  def change
    #@users = User.all
    @users = User.order('username ASC')
    @usersite = Usersite.all
    @site = Site.all
    userId = params[:user]
     newrole = params[:role]
     u = User.find(userId)
     u.role_id = newrole
     u.save
     render "users/status"
  end
  def addusertosite
    #@users = User.all
    @users = User.order('username ASC')
    @usersite = Usersite.all
    @site = Site.all
    user = params[:user]
    site = params[:site]
    @usersite = Usersite.new
    @usersite.site_id = site.to_s
    @usersite.user_id = user.to_s
    @usersite.add = "0"
    @usersite.edit = "0"
    @usersite.remove = "0"
    @usersite.save
    render :text => "DONE"
  end
  def sitepermission
    @users = User.all
    @usersite = Usersite.all
    @site = Site.all
    user = params[:user]
    site = params[:site]
    val = params[:val]
    chnage = params[:do]
    if chnage == "0"
      @usersite1 = Usersite.where('user_id = ? AND site_id = ?', user, site).first
      @usersite1.add = val.to_s
      @usersite1.save
    end
    if chnage == "1"
      @usersite1 = Usersite.where('user_id = ? AND site_id = ?', user, site).first
      @usersite1.edit = val.to_s
      @usersite1.save
    end
    if chnage == "2"
      @usersite1 = Usersite.where('user_id = ? AND site_id = ?', user, site).first
      @usersite1.remove = val.to_s
      @usersite1.save

    end
    render :text => "DONE"
  end
end


