class HomographyController < ApplicationController
  skip_before_filter  :verify_authenticity_token
  def calchm
    task = params[:task].to_s
    values = params[:values].to_s
    divide = params[:divide].to_s
    cmd = "cd software/homography && ./homography " + task + " " + divide + " " + values;
    result = `#{cmd}`
    render :text => result
  end
end

