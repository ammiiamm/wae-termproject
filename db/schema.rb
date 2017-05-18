# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20161205123024) do

  # These are extensions that must be enabled in order to support this database
  enable_extension "plpgsql"

  create_table "advertisements", force: :cascade do |t|
    t.string   "add_type"
    t.string   "contecnt"
    t.boolean  "status"
    t.integer  "user_id"
    t.integer  "site_id"
    t.integer  "zone_id"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  add_index "advertisements", ["site_id"], name: "index_advertisements_on_site_id", using: :btree
  add_index "advertisements", ["user_id"], name: "index_advertisements_on_user_id", using: :btree
  add_index "advertisements", ["zone_id"], name: "index_advertisements_on_zone_id", using: :btree

  create_table "cameras", force: :cascade do |t|
    t.string   "name"
    t.string   "ip_address"
    t.string   "cam_type"
    t.string   "lense"
    t.string   "config"
    t.string   "other_info"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "configurations", force: :cascade do |t|
    t.string   "name"
    t.boolean  "config_status"
    t.integer  "site_id"
    t.integer  "user_id"
    t.integer  "camera_id"
    t.integer  "zone_id"
    t.string   "slotconfiguration"
    t.string   "roihomo"
    t.datetime "created_at",        null: false
    t.datetime "updated_at",        null: false
  end

  add_index "configurations", ["camera_id"], name: "index_configurations_on_camera_id", using: :btree
  add_index "configurations", ["site_id"], name: "index_configurations_on_site_id", using: :btree
  add_index "configurations", ["user_id"], name: "index_configurations_on_user_id", using: :btree
  add_index "configurations", ["zone_id"], name: "index_configurations_on_zone_id", using: :btree

  create_table "letsencrypt_plugin_challenges", force: :cascade do |t|
    t.text     "response"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "letsencrypt_plugin_settings", force: :cascade do |t|
    t.text     "private_key"
    t.datetime "created_at",  null: false
    t.datetime "updated_at",  null: false
  end

  create_table "parkingslots", force: :cascade do |t|
    t.integer  "zone_id"
    t.string   "name"
    t.boolean  "status"
    t.point    "point1"
    t.point    "point2"
    t.point    "point3"
    t.point    "point4"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  add_index "parkingslots", ["zone_id"], name: "index_parkingslots_on_zone_id", using: :btree

  create_table "parkingtotals", force: :cascade do |t|
    t.integer  "zone_id"
    t.string   "free"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  add_index "parkingtotals", ["zone_id"], name: "index_parkingtotals_on_zone_id", using: :btree

  create_table "roles", force: :cascade do |t|
    t.string   "name"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "sites", force: :cascade do |t|
    t.string   "name"
    t.string   "address"
    t.string   "districs"
    t.string   "city"
    t.string   "postalcode"
    t.integer  "user_id"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.string   "Long"
    t.string   "Lat"
  end

  add_index "sites", ["user_id"], name: "index_sites_on_user_id", using: :btree

  create_table "users", force: :cascade do |t|
    t.string   "email",                  default: "", null: false
    t.string   "encrypted_password",     default: "", null: false
    t.string   "reset_password_token"
    t.datetime "reset_password_sent_at"
    t.datetime "remember_created_at"
    t.integer  "sign_in_count",          default: 0,  null: false
    t.datetime "current_sign_in_at"
    t.datetime "last_sign_in_at"
    t.inet     "current_sign_in_ip"
    t.inet     "last_sign_in_ip"
    t.datetime "created_at",                          null: false
    t.datetime "updated_at",                          null: false
    t.string   "username"
    t.integer  "role_id"
  end

  add_index "users", ["email"], name: "index_users_on_email", unique: true, using: :btree
  add_index "users", ["reset_password_token"], name: "index_users_on_reset_password_token", unique: true, using: :btree
  add_index "users", ["role_id"], name: "index_users_on_role_id", using: :btree

  create_table "usersites", force: :cascade do |t|
    t.integer  "user_id"
    t.integer  "site_id"
    t.string   "add"
    t.string   "edit"
    t.string   "remove"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  add_index "usersites", ["site_id"], name: "index_usersites_on_site_id", using: :btree
  add_index "usersites", ["user_id"], name: "index_usersites_on_user_id", using: :btree

  create_table "zones", force: :cascade do |t|
    t.string   "name"
    t.integer  "site_id"
    t.integer  "numberofslots"
    t.integer  "numberfree"
    t.integer  "user_id"
    t.datetime "created_at",    null: false
    t.datetime "updated_at",    null: false
    t.string   "Lat"
    t.string   "Long"
  end

  add_index "zones", ["site_id"], name: "index_zones_on_site_id", using: :btree
  add_index "zones", ["user_id"], name: "index_zones_on_user_id", using: :btree

  add_foreign_key "advertisements", "sites"
  add_foreign_key "advertisements", "users"
  add_foreign_key "advertisements", "zones"
  add_foreign_key "configurations", "cameras"
  add_foreign_key "configurations", "sites"
  add_foreign_key "configurations", "users"
  add_foreign_key "configurations", "zones"
  add_foreign_key "parkingslots", "zones"
  add_foreign_key "parkingtotals", "zones"
  add_foreign_key "sites", "users"
  add_foreign_key "users", "roles"
  add_foreign_key "usersites", "sites"
  add_foreign_key "usersites", "users"
  add_foreign_key "zones", "sites"
  add_foreign_key "zones", "users"
end
