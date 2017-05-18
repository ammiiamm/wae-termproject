class ApplicationController < ActionController::Base
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
  protect_from_forgery with: :exception

  rescue_from CanCan::AccessDenied do |exception|
    flash[:alert] = "Access denied. You do not have a privileges to access your requested page."
    redirect_to root_url
  end

  before_filter :update_sanitized_params, if: :devise_controller?
  def update_sanitized_params
    devise_parameter_sanitizer.permit(:sign_up, keys: [:username, :email, :password])
  end

  def after_sign_in_path_for(resource)
    welcome_index_path #main index
  end

  def after_sign_out_path_for(resource)
    root_path
  end
end
